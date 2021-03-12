/**
    Definition of Monitor
*/

#include "Monitor.hpp"

#include <dlib/image_processing.h> 						 	// dlib::shape_predictor
#include <dlib/image_processing/frontal_face_detector.h>	// dlib::frontal_face_detector
#include <dlib/opencv.h>									// dlib::cv_image, dlib::bgr_pixel
#include <opencv2/opencv.hpp>								// cv::VideoCapture

#include "Eye.hpp"

// Threshold parameters for detecting blinks
const double EYE_ASPECT_RATIO_THRESHOLD = 0.2;
const int EYE_ASPECT_RATIO_CONSECUTIVE_FRAMES = 2;

/**
    Constructor
*/
Monitor::Monitor()
    : mExitMonitoring( false )
{
}

/**
    Starts thread for monitoring eyes
*/
void Monitor::Start()
{
    mThread = std::thread( &Monitor::TrackEyes, this );
}

/**
    Closes eye tracking functionality
*/
void Monitor::Stop()
{
    mExitMonitoring = true;
    if( mThread.joinable() )
    {
        mThread.join();
    }
}

/**
    Registers callback for mUserBlinked signal

    @return connection to mUserBlinked signal
*/
boost::signals2::connection Monitor::RegisterUserBlinked
    (
    boost::signals2::signal<void ()>::slot_type const& aSlot
    )
{
    return mUserBlinked.connect( aSlot );
}

/**
    Tracks users eyes and sends mUserBlinked signal every time user blinks

	Method of determining if eyes are open or not is described in this paper:
	http://vision.fe.uni-lj.si/cvww2016/proceedings/papers/05.pdf. Using landmarks on the face
	surrounding the eye, the eye aspect ratio is calculated which represents the ratio of the
	height of the eye to the width of the eye. If this ratio falls below the set threshold in
	a particular frame, the counter is incremented. When this ratio is not below the set threshold,
	the number of consecutive frames the eye was closed is checked. If the number of consecutive
	frames is above the set threhold the user has blinked, otherwise the counter is reset.
*/
void Monitor::TrackEyes()
{
	// Open webcam for detecting blinks
	cv::VideoCapture videoCapture( 0 );
	if( !videoCapture.isOpened() )
	{
		return;
	}

	// Tracks how many consecutive frames eye aspect ratio is below EYE_ASPECT_RATIO_THRESHOLD
	int counter = 0;

	// Get dlib facial detector
	dlib::frontal_face_detector facialDetector = dlib::get_frontal_face_detector();

	// Get predictor that maps points onto the face
	dlib::shape_predictor shapePredictor;
	dlib::deserialize( "../include/shape_predictor_68_face_landmarks.dat" ) >> shapePredictor;

	// Face with all 68 points mapped onto it
	dlib::full_object_detection face;

	while( !mExitMonitoring )
	{
		// Capture single frame of video
		cv::Mat frame;
		videoCapture >> frame;
		dlib::cv_image<dlib::bgr_pixel> cimg( frame );

		// Detect faces in frame
		std::vector<dlib::rectangle> faces = facialDetector( cimg );

		if( faces.size() == 1 )
		{
			face = shapePredictor( cimg, faces[0] );

			// Point indicies surrounding left and right eyes can be found in the following
			// article: https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/
			// Points on diagram are one-based indicies, zero-based below

			Eye leftEye
				(
				face.part( 36 ).x(), face.part( 36 ).y(),
				face.part( 37 ).x(), face.part( 37 ).y(),
				face.part( 38 ).x(), face.part( 38 ).y(),
				face.part( 39 ).x(), face.part( 39 ).y(),
				face.part( 40 ).x(), face.part( 40 ).y(),
				face.part( 41 ).x(), face.part( 41 ).y()
				);

			Eye rightEye
				(
				face.part( 36 ).x(), face.part( 36 ).y(),
				face.part( 37 ).x(), face.part( 37 ).y(),
				face.part( 38 ).x(), face.part( 38 ).y(),
				face.part( 39 ).x(), face.part( 39 ).y(),
				face.part( 40 ).x(), face.part( 40 ).y(),
				face.part( 41 ).x(), face.part( 41 ).y()
				);

			double averagedEyeAspectRatio =
				( leftEye.AspectRatio() + rightEye.AspectRatio() ) / 2.0;

			if( averagedEyeAspectRatio < EYE_ASPECT_RATIO_THRESHOLD )
			{
				// Blink detected
				++counter;
			}
			else
			{
				// Check if eye was closed for required number of frames
				if( counter >= EYE_ASPECT_RATIO_CONSECUTIVE_FRAMES )
				{
					mUserBlinked();
				}
				counter = 0;
			}
		}
	}
}