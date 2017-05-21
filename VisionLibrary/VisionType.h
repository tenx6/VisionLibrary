#ifndef _VISION_TYPE_H_
#define _VISION_TYPE_H_

namespace AOI
{
namespace Vision
{

#define MAX_NUM_OF_DEFECT_CRITERIA				(5)
#define MAX_NUM_OF_DEFECT_RESULT				(20)
#define PARALLEL_LINE_SLOPE_DIFF_LMT			(0.1)
#define PARALLEL_LINE_MERGE_DIST_LMT			(20)
#define PR_ELECTRODE_COUNT                      (2)
#define PR_MAX_GRAY_LEVEL                       (255)
#define PR_MIN_GRAY_LEVEL                       (0)
#define PR_RECT_EDGE_COUNT                      (4)
#define PR_MAX_ERR_STR_LEN						(100)
#define PR_AUTO_THRESHOLD_MAX_NUM               (4)
#define PR_FIT_LINE_MAX_POINT_COUNT             (10000)
static const double PR_MM_TO_UM              =  1000.;

enum class PR_SRCH_OBJ_ALGORITHM {
	SIFT,
	SURF,
};

enum class PR_DEFECT_ATTRIBUTE {
	BRIGHT, // Only bright defect (those defects brighter than the reference and the 
	// absolute value of the difference is larger than the defect threshold) is detected. 
	DARK,	// Only dark defect (those defects darker than the reference and the absolute 
	// value of the difference is larger than the defect threshold) is detected
	BOTH,	// Both bright and dark defect is detected
	BRIGHT_LOOP,	// special for AOI
	DARK_LOOP,		// special for AOI
	END
};

enum class PR_DEFECT_TYPE {
	BLOB,
	LINE,
	END,
};

enum class PR_DEBUG_MODE {
    DISABLED,
    SHOW_IMAGE,
    LOG_ALL_CASE,
    LOG_FAIL_CASE,
};

enum class PR_RECORD_TYPE {
    INVALID = -1,
    OBJECT,
    DEVICE,
    CHIP,
    LEAD,
};

enum class PR_FIDUCIAL_MARK_TYPE {
    SQUARE,
    CIRCLE,
};

enum class PR_RM_FIT_NOISE_METHOD {
    ABSOLUTE_ERR,
    POSITIVE_ERR,
    NEGATIVE_ERR,
};

enum class PR_EDGE_DIRECTION {
    HORIZONTAL,
    VERTIAL,
    ALL,
};

enum class PR_FIT_CIRCLE_METHOD {
    LEAST_SQUARE,
    LEAST_SQUARE_REFINE,
    RANSAC,
};

enum class PR_DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

enum class PR_OBJECT_ATTRIBUTE {
    BRIGHT,
	DARK,
    END,
};

enum class PR_FILTER_TYPE {
    NORMALIZED_BOX_FILTER,
    GAUSSIAN_FILTER,
    MEDIAN_FILTER,
    BILATERIAL_FILTER,
    END,
};

enum class PR_COMPARE_TYPE {
    SMALLER,
    LARGER,
};

enum class PR_DETECT_LINE_DIR {
    MIN_TO_MAX,
    MAX_TO_MIN,
};

enum class PR_FILL_HOLE_METHOD {
    CONTOUR,
    MORPHOLOGY,
};

enum class PR_OBJECT_MOTION {
    TRANSLATION,    //Only move in XY
    EUCLIDEAN,      //Move and rotation in XY
    AFFINE,         //Move and rotation in XY, and have one direction shape change.
    HOMOGRAPHY      //Move and rotation in XY, and have multiple shape change.
};

enum class PR_INSP_BRIDGE_MODE {
    INNER,
    OUTER,
};

enum class PR_INSP_BRIDGE_DIRECTION {
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

enum class PR_INSP_CHIP_MODE {
    HEAD,
    BODY,
    SQUARE,
    RNET,
    CAE,
    CIRCULAR,
};

}
}

#endif