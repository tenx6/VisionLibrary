#ifndef _AOI_VISION_ALGORITHM_H
#define _AOI_VISION_ALGORITHM_H

#include "BaseType.h"
#include "opencv2/core/core.hpp"
#include "VisionHeader.h"
#include "StopWatch.h"
#include <list>
#include <memory>

using namespace std;

namespace AOI
{
namespace Vision
{

#define RADIAN_2_DEGREE(RADIAN)	( RADIAN * 180.f / CV_PI )
#define DEGREE_2_RADIAN(DEGREE) ( DEGREE * CV_PI / 180.f )

class VisionAlgorithm : private Uncopyable
{
public:
    explicit VisionAlgorithm();
    static shared_ptr<VisionAlgorithm> create();
    VisionStatus lrnTmpl(PR_LEARN_TMPL_CMD * const pLearnTmplCmd, PR_LEARN_TMPL_RPY *pLearnTmplRpy);
    VisionStatus srchTmpl(PR_SRCH_TMPL_CMD *const pFindObjCmd, PR_SRCH_TMPL_RPY *pFindObjRpy);
    VisionStatus inspDevice(PR_INSP_DEVICE_CMD *pstInspDeviceCmd, PR_INSP_DEVICE_RPY *pstInspDeivceRpy);
    int align(PR_AlignCmd *const pAlignCmd, PR_AlignRpy *pAlignRpy);
	int inspect(PR_InspCmd *const pInspCmd, PR_InspRpy *pInspRpy);
	static float distanceOf2Point(const cv::Point &pt1, const cv::Point &pt2);
	static float distanceOf2Point(const cv::Point2f &pt1, const cv::Point2f &pt2);
protected:
	int _findBlob(const cv::Mat &mat, const cv::Mat &matRevs, PR_InspCmd *const pInspCmd, PR_InspRpy *pInspRpy );
	int _findLine(const cv::Mat &mat, PR_InspCmd *const pInspCmd, PR_InspRpy *pInspRpy );
	int _mergeLines(const vector<PR_Line2f> &vecLines, vector<PR_Line2f> &vecResultLines);
	int _merge2Line(const PR_Line2f &line1, const PR_Line2f &line2, PR_Line2f &lineResult);
	int _findLineCrossPoint(const PR_Line2f &line1, const PR_Line2f &line2, cv::Point2f ptResult);
    VisionStatus _writeLrnTmplRecord(PR_LEARN_TMPL_RPY *pLearnTmplRpy);
protected:
    const int       _constMinHessian        =       300;
    const int       _constOctave            =       4;
    const int       _constOctaveLayer       =       3;
    const UInt32    _constLeastFeatures     =       3;
    const String    _strRecordLogPrefix     =       "tmplDir.";
    const float     _constExpSmoothRatio    =       0.3f;
    cv::Mat         _mat;
    PR_DEBUG_MODE   _enDebugMode        = PR_DEBUG_MODE::DISABLED;
    CStopWatch      _stopWatch;
};

}
}
#endif