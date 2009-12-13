#include "Misc.h"

namespace dc{
    /** \brief Bilinear interpolate on the matrix to find the value at point

	  There are times where the src_matrix actually repeat in a specific direction (e.g. x). When this happens,
	  you should create a copy of src_matrix with one extra column. This new column will be the same as the
	  first column of the src matrix => creating the effect of repeating in x.

      \param src_matrix a CV_8UC1 matrix
      \param point target point (a 2x1 float)
	  
    */
	float bilinear_interpolate( const Mat & src_matrix, const Mat & point )
	{
        float temp_low, temp_high;

        float x = point.at<float>(0,0);
        float y = point.at<float>(1,0);

        // sanity check
        if (x<0 || x> (src_matrix.cols-1) || y<0 || y>(src_matrix.rows-1)){
            return 0;
        }

        // now we interpolate
        int x_low = (int) floor(x);
        int x_high = x_low + 1;
        int y_low = (int) floor(y);
        int y_high = y_low + 1;

        // handle points at maximum
        if (x_high > src_matrix.cols - 1){
            if (y_high > src_matrix.rows - 1){
                return static_cast<float>(src_matrix.at<uchar>(y_low, x_low));
            } else {
                temp_low = src_matrix.at<uchar>(y_low, x_low);
                temp_high = src_matrix.at<uchar>(y_high, x_low);
            }
        } else {
			if (y_high > src_matrix.rows - 1){
				temp_low = src_matrix.at<uchar>(y_low, x_low);
				temp_high = src_matrix.at<uchar>(y_low, x_high);
			} else {
				// most common case
				// interpolate in x
				temp_low = src_matrix.at<uchar>(y_low,x_low)*(x_high - x) + src_matrix.at<uchar>(y_low,x_high)*(x - x_low);
				temp_high = src_matrix.at<uchar>(y_high,x_low)*(x_high - x) + src_matrix.at<uchar>(y_high,x_high)*(x - x_low);
			}
            
        }

        // interpolate in y
        return temp_low * (y_high - y) + temp_high * (y - y_low);
    }

    /** \brief Rotate the src image and put it in the dst image
	\param mtx rotation matrix [cosa -sina; sina cosa]
	*/
    void rotate(const Mat& src, Mat& dst, const Point & axis_of_rotation, const Mat& mtx){
        // sanity check
        if (mtx.cols !=2 || mtx.rows !=2)
            throw invalid_argument("Rotation matrix has to be a 2x2 float matrix");

        // clear destination matrix
        dst = Mat::zeros(dst.size(), CV_8U);

        // find the inverse transformation matrix
        Mat inv_mtx = mtx.inv();

        // perform inverse transformation
        Mat dst_points = Mat(2,1,CV_32F);
        Mat src_points = Mat(2,1,CV_32F);

        for(int y=0; y<src.rows; y++){
            for (int x=0; x<src.cols; x++){
                dst_points.at<float>(0,0) = x - axis_of_rotation.x;
                dst_points.at<float>(1,0) = y - axis_of_rotation.y;

                src_points = inv_mtx * dst_points;

                // translate to compensate for axis of rotation
                src_points.at<float>(0,0) += axis_of_rotation.x;
                src_points.at<float>(1,0) += axis_of_rotation.y;

                printf("Invert transform: dst (%f, %f) => src (%f, %f)\n",
                       dst_points.at<float>(0,0), dst_points.at<float>(1,0),
                       src_points.at<float>(0,0), src_points.at<float>(1,0));

                // we interpolate to find the resulting intensity
                dst.at<uchar>(y,x) = (uchar)bilinear_interpolate(src,src_points);
            }
        }
    }

    /** \brief Rotate the src image theta degree (CCW from x-axis) and put it in dst
	\param theta angle in radian
	\note http://mathworld.wolfram.com/RotationMatrix.html

        \bug It seems that the angle is given as clockwise instead of counter-clockwise
	*/
    void rotate(const Mat& src, Mat& dst, const Point & axis_of_rotation, float theta){
        Mat rotation_matrix = Mat(2,2,CV_32F);
        rotation_matrix.at<float>(0,0) = cos(theta);
        rotation_matrix.at<float>(0,1) = -sin(theta);
        rotation_matrix.at<float>(1,0) = sin(theta);
        rotation_matrix.at<float>(1,1) = cos(theta);

        rotate(src, dst, axis_of_rotation, rotation_matrix);
    }

    /** \brief Contrast stretch an image
        \param src a CV_64F image
        \return Output is a CV_8U Mat
    */
    Mat contrast_stretch_to_image(const Mat & src){
        Mat output = Mat(src.size(), CV_8U);
        double min_val, max_val;
        minMaxLoc(src, &min_val, &max_val);

        double stretch_ratio = 255/ (max_val - min_val);
        for (int i=0; i<output.rows; i++){
            for (int j=0; j<output.cols; j++){
                output.at<uchar>(i,j) = (src.at<double>(i,j)-min_val) * stretch_ratio;
            }
        }
        return output;
    }



    /** \brief Perform swapping of quadrant 1 and 3, 2 and 4
        \param input uchar mat

        \bug Only work with size that are even
      */
    Mat fftshift(const Mat & input){
        Mat output = input.clone();
        int half_x = input.rows / 2;
        int half_y = input.cols / 2;
        int new_i, new_j;
        for(int i=0; i<input.rows; i++){
            for(int j=0; j<input.cols; j++){
                new_i = (i + half_x) % input.rows;
                new_j = (j + half_y) % input.cols;
                output.at<uchar>(new_i,new_j) = input.at<uchar>(i,j);
            }
        }
        return output;
    }

	/** \brief Perform cartesian to polar image mapping
	\note While named logPolar, this method does NOT perform log at all.
	*/
    Mat logPolar(const Mat & image, int size_rho, int size_theta, Point center){
        const double radian_per_theta_step = 2*dc::pi / (size_theta);

        Mat logpolar_image = Mat::zeros(Size(size_theta,size_rho), CV_8U);

        // perform inverse mapping
        // we know that
        //   x = rho * cos(theta)
        //   y = rho * sin(theta)
        Mat target_point = Mat(2,1,CV_32F);
        for (int rho=0; rho<logpolar_image.rows; rho++){
            for (int theta=0; theta<logpolar_image.cols; theta++){
                target_point.at<float>(0,0) = center.x + float(rho) * cos(theta * radian_per_theta_step);
                target_point.at<float>(1,0) = center.y + float(rho) * sin(theta * radian_per_theta_step);

                // perform bilinear interpolation
//				std::cout << rho << " " << theta << " " << target_point.at<float>(0,0) << " " << target_point.at<float>(1,0) << endl;
                logpolar_image.at<uchar>(rho, theta) = static_cast<uchar>(dc::bilinear_interpolate(image,target_point));
            }
        }

        return logpolar_image;
    }
}
