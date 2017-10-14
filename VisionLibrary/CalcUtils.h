#ifndef _CALC_UTILS_H_
#define _CALC_UTILS_H_

#include "BaseType.h"
#include "VisionHeader.h"
#include <math.h>

namespace AOI
{
namespace Vision
{

class CalcUtils
{
private:
    CalcUtils();
    CalcUtils(CalcUtils const &);
    CalcUtils(CalcUtils const &&);
    CalcUtils &operator=(CalcUtils const &);        
    ~CalcUtils();
public:
    template<typename T>
    static inline float distanceOf2Point(const cv::Point_<T> &pt1, const cv::Point_<T> &pt2)
    {
        float fOffsetX = ( float ) pt1.x - pt2.x;
        float fOffsetY = ( float ) pt1.y - pt2.y;
        float fDistance = sqrt(fOffsetX * fOffsetX + fOffsetY * fOffsetY);
        return fDistance;
    }

    template<typename T>
    static inline double calcSlopeDegree(const cv::Point_<T> &pt1, const cv::Point_<T> &pt2)
    {
        return radian2Degree ( atan2f ( ToFloat( pt2.y - pt1.y ), ToFloat ( pt2.x - pt1.x ) ) );
    }

    template<typename T>
    static inline cv::Point_<T>midOf2Points(const cv::Point_<T> &pt1, const cv::Point_<T> &pt2)
    {
        return cv::Point_<T>( ( pt1.x + pt2.x ) / 2.f, ( pt1.y + pt2.y ) / 2.f );
    }

    template<typename T>
    static inline cv::Rect_<T> scaleRect(const cv::Rect_<T> &rectInput, float fScale)
    {
        return cv::Rect_<T> (cv::Point_<T>(rectInput.x + ( 1.f - fScale ) * rectInput.width, rectInput.y + ( 1.f - fScale ) * rectInput.height ),
            cv::Point_<T>(rectInput.x + ( 1.f + fScale ) * rectInput.width, rectInput.y + ( 1.f + fScale ) * rectInput.height ) );
    }

    template<typename T>
    static inline cv::Rect_<T> resizeRect(const cv::Rect_<T> &rectInput, cv::Size_<T> szNew)
    {
        return cv::Rect_<T> (rectInput.x + rectInput.width / 2.f - szNew.width / 2, rectInput.y + rectInput.height / 2.f  - szNew.height / 2,
            szNew.width, szNew.height );
    }

    template<typename T>
    static inline cv::Rect_<T> Rect(const cv::Rect_<T> &rectInput, float fScale)
    {
        return cv::Rect_<T> (cv::Point_<T>(rectInput.x + ( 1.f - fScale ) * rectInput.width, rectInput.y + ( 1.f - fScale ) * rectInput.height ),
            cv::Point_<T>(rectInput.x + ( 1.f + fScale ) * rectInput.width, rectInput.y + ( 1.f + fScale ) * rectInput.height ) );
    }

    template<typename Tp>
    static inline Tp mean (const std::vector<Tp> &vecInput) {
        if ( vecInput.empty() )
            return 0;
        Tp sum = 0;
        for ( auto value : vecInput )
            sum += value;
        return sum / vecInput.size();
    }

    //Calculate deviation in one pass. The algorithm is get from https://www.strchr.com/standard_deviation_in_one_pass
    template<typename T>
    static double calcStdDeviation(std::vector<T> vecValue)
    {
        if (vecValue.empty())
            return 0.0;
        double sum = 0;
        double sq_sum = 0;
        for (const auto value : vecValue) {
            sum += value;
            sq_sum += value * value;
        }
        auto n = vecValue.size();
        double mean = sum / n;
        double variance = sq_sum / n - mean * mean;
        return sqrt(variance);
    }

    template<typename _Tp>
    static inline cv::Mat genMaskByValue(const cv::Mat &matInputImg, const _Tp value) {
        cv::Mat matResultImg;
        cv::compare ( matInputImg, value, matResultImg, cv::CmpTypes::CMP_EQ );
        return matResultImg;
    }    

    template<typename _Tp>
    static inline cv::Point2f warpPoint(const cv::Mat &matWarp, const cv::Point2f &ptInput) {
        cv::Mat matPoint = cv::Mat::zeros(3, 1, matWarp.type());
        matPoint.at<_Tp>(0, 0) = ptInput.x;
        matPoint.at<_Tp>(1, 0) = ptInput.y;
        matPoint.at<_Tp>(2, 0) = 1.f;
        cv::Mat matResultImg = matWarp * matPoint;
        return cv::Point_<_Tp> ( ToFloat ( matResultImg.at<_Tp>(0, 0) ), ToFloat ( matResultImg.at<_Tp>(1, 0) ) );
    }

    template<typename _Tp>
    static inline VectorOfPoint2f warpRect(const cv::Mat &matWarp, const cv::Rect2f &rectInput) {
        VectorOfPoint2f vecPoints;
        vecPoints.push_back ( warpPoint<_Tp>( matWarp, rectInput.tl() ) );
        vecPoints.push_back ( warpPoint<_Tp>( matWarp, cv::Point2f(rectInput.x + rectInput.width, rectInput.y ) ) );
        vecPoints.push_back ( warpPoint<_Tp>( matWarp, rectInput.br() ) );
        vecPoints.push_back ( warpPoint<_Tp>( matWarp, cv::Point2f(rectInput.x, rectInput.y + rectInput.height ) ) );
        return vecPoints;
    }

    template<typename _Tp>
    static inline std::vector<std::vector<_Tp>> matToVector(const cv::Mat &matInputImg) {
        std::vector<std::vector<_Tp>> vecVecArray ( matInputImg.rows, std::vector<_Tp>(matInputImg.cols, 0 ) );
        if ( matInputImg.isContinuous () ) {
            for ( int row = 0; row < matInputImg.rows; ++ row ) {
                int nRowStart = row * matInputImg.cols;
                vecVecArray[row].assign ( (_Tp *)matInputImg.datastart + nRowStart, (_Tp *)matInputImg.datastart + nRowStart + matInputImg.cols );
            }
        }else {
            for ( int row = 0; row < matInputImg.rows; ++ row ) {
                vecVecArray[row].assign ( (_Tp*)matInputImg.ptr<uchar> ( row ), (_Tp*)matInputImg.ptr<uchar> ( row ) + matInputImg.cols );
            }
        }
        return vecVecArray;
    }

    template<typename _Tp>
    static inline cv::Mat vectorToMat(const std::vector<std::vector<_Tp>> &vecVecArray) {
        cv::Mat matResult( ToInt32 ( vecVecArray.size() ), ToInt32 ( vecVecArray[0].size() ), CV_32FC1 );
        int nRow = 0;
        for (const auto &vecRow : vecVecArray) {
            cv::Mat matTarget ( matResult, cv::Range ( nRow, nRow + 1 ), cv::Range::all () );
            cv::Mat matSrc ( cv::Size ( matResult.cols, 1 ), CV_32FC1, (void *)vecRow.data () );
            matSrc.copyTo ( matTarget );
            ++ nRow;
        }
        return matResult;
    }

    //if nDimension == 1, returns the cumulative sum of each col, otherwise return the cumulative sum of each row.
    template<typename _Tp>
    static inline cv::Mat cumsum ( const cv::Mat &matInput, int nDimension ) {
        cv::Mat matResult = matInput.clone ();
        const int ROWS = matInput.rows;
        const int COLS = matInput.cols;
        if( nDimension == 1 ) {
            for ( int row = 0; row < ROWS - 1; ++ row ) {
                cv::Mat matCurrentRow( matResult, cv::Rect (0, row,     COLS, 1 ) );
                cv::Mat matNextRow   ( matResult, cv::Rect (0, row + 1, COLS, 1 ) );
                matNextRow = matNextRow + matCurrentRow;
            }
        }else {
            //Use the matrix add is not optimized for cache access, so it is slower than the one by one add.
            //for ( int col = 0; col < COLS - 1; ++ col ) {
            //    cv::Mat matCurrentCol ( matResult, cv::Rect ( col,     0, 1, ROWS ) );
            //    cv::Mat matNextCol    ( matResult, cv::Rect ( col + 1, 0, 1, ROWS ) );
            //    matNextCol = matNextCol + matCurrentCol;
            //}
            for( int row = 0; row < matResult.rows; ++ row ) {
                for( int col = 0; col < matResult.cols - 1; ++ col ) {
                    matResult.at<_Tp> ( row, col + 1 ) += matResult.at<_Tp> ( row, col ) ;
                }
            }
        }
        return matResult;
    }

    template<typename _tp>
    static void meshgrid ( _tp xStart, _tp xInterval, _tp xEnd, _tp yStart, _tp yInterval, _tp yEnd, cv::Mat &matX, cv::Mat &matY ) {
        cv::Mat matCol = intervals<_tp>(xStart, xInterval, xEnd);
        matCol = matCol.reshape ( 1, 1 );

        cv::Mat matRow = intervals<_tp>(yStart, yInterval, yEnd);
        matX = cv::repeat ( matCol, matRow.rows, 1 );
        matY = cv::repeat ( matRow, 1, matCol.cols );
    }

    template<typename _tp>
    static cv::Mat intervals ( _tp start, _tp interval, _tp end ) {
        std::vector<_tp> vecValue;
        int nSize = ToInt32 ( ( end - start ) / interval );
        if ( nSize <= 0 ) {
            static String msg = String ( __FUNCTION__ ) + " input paramters are invalid.";
            throw std::exception( msg.c_str() );
        }
        vecValue.reserve ( nSize );
        _tp value = start;
        if ( interval > 0 ) {
            while ( value <= end ) {
                vecValue.push_back ( value );
                value += interval;
            }
        }else {
            while ( value >= end ) {
                vecValue.push_back ( value );
                value += interval;
            }
        }
        //cv::Mat matResult ( vecValue ); //This have problem, because matResult share the memory with vecValue, after leave this function, the memory already released.
        return cv::Mat(vecValue).clone();
    }

    template<typename T>
    static inline cv::Mat floor ( const cv::Mat &matInput ) {
        cv::Mat matResult = matInput.clone ();
        for( int i = 0; i < matResult.total(); ++ i ) {
            T &value = matResult.at<T> ( i );
            value = std::floor ( value );
        }
        return matResult;
    }

    template<typename T>
    static inline void floorByRef ( cv::Mat &matInOut ) {
        for( int i = 0; i < matInOut.total(); ++ i ) {
            T &value = matInOut.at<T> ( i );
            value = std::floor ( value );
        }
    }

    static inline cv::Mat getNanMask(const cv::Mat &matInput) {
        cv::Mat matResult = cv::Mat ( matInput == matInput );
        matResult = 255 - matResult;
        return matResult;
    }

    template<typename T>
    static cv::Mat sin(const cv::Mat &matInput) {
        cv::Mat matResult ( matInput.rows, matInput.cols, matInput.type () );
        for( int row = 0; row < matInput.rows; ++ row )
        for( int col = 0; col < matInput.cols; ++ col )
            matResult.at<T> ( row, col ) = std::sin ( matInput.at<T> ( row, col ) );
        return matResult;
    }

    template<typename T>
    static cv::Mat cos(const cv::Mat &matInput) {
        cv::Mat matResult ( matInput.rows, matInput.cols, matInput.type () );
        for( int row = 0; row < matInput.rows; ++ row )
        for( int col = 0; col < matInput.cols; ++ col )
            matResult.at<T> ( row, col ) = std::cos ( matInput.at<T> ( row, col ) );
        return matResult;
    }

    // To heapify a subtree rooted with node i which is
    // an index in arr[]. n is size of heap
    template<typename T>
    static void heapifyMinToRoot ( std::vector<T> &vecInput, const int n, const int i, std::vector<size_t> &vecIndex ) {
        int smallestIndex = i;  // Initialize smallest as root
        int l = 2 * i + 1;  // left = 2*i + 1
        int r = 2 * i + 2;  // right = 2*i + 2

        // If left child is larger than root
        if (l < n && vecInput[l] < vecInput[smallestIndex])
            smallestIndex = l;

        // If right child is larger than largest so far
        if (r < n && vecInput[r] < vecInput[smallestIndex])
            smallestIndex = r;

        // If largest is not root
        if (smallestIndex != i)
        {
            std::swap ( vecInput[i], vecInput[smallestIndex] );
            std::swap ( vecIndex[i], vecIndex[smallestIndex] );

            // Recursively heapify the affected sub-tree
            heapifyMinToRoot ( vecInput, n, smallestIndex, vecIndex );
        }
    }

    template<typename T>
    static std::vector<T> FindLargestKItems ( const cv::Mat &matInput, const int K, std::vector<size_t> &vecIndex ) {
        if ( K > matInput.total () )  {
            return matInput;
        }

        std::vector<T> vecResult;
        vecResult.reserve ( K );
        int index = 0;
        while (vecResult.size () < K) {
            if (matInput.at<T> ( index ) == matInput.at<T> ( index )) {
                vecIndex.push_back ( index );
                vecResult.push_back ( matInput.at<T> ( index ) );
            }
            ++ index;
        }

        for ( int K1 = K / 2 - 1; K1 >= 0; -- K1 )
            heapifyMinToRoot ( vecResult, ToInt32 ( K ), K1, vecIndex );

        for (int i = index; i < ToInt32 ( matInput.total() ); ++i) {
            if ( matInput.at<T> ( i ) == matInput.at<T> ( i ) && matInput.at<T> ( i ) > vecResult.front() ) {
                vecResult.front() = matInput.at<T> ( i );
                vecIndex.front() = i;

                for (int K1 = K / 2 - 1; K1 >= 0; -- K1 )
                    heapifyMinToRoot ( vecResult, ToInt32 ( K ), K1, vecIndex );
            }
        }

        for (int k = K - 1; k >= 0; -- k)
        {
            std::swap ( vecResult[k], vecResult[0] );
            std::swap ( vecIndex[k], vecIndex[0] );

            heapifyMinToRoot<T> ( vecResult, k, 0, vecIndex );
        }
        return vecResult;
    }

    static inline cv::Mat diff ( const cv::Mat &matInput, int nRecersiveTime, int nDimension ) {
        const int DIFF_ON_Y_DIR = 1;
        const int DIFF_ON_X_DIR = 2;
        assert ( DIFF_ON_X_DIR == nDimension || DIFF_ON_Y_DIR == nDimension );
        if (nRecersiveTime > 1)
            return diff ( diff ( matInput, nRecersiveTime - 1, nDimension ), 1, nDimension );

        cv::Mat matKernel;
        if (DIFF_ON_X_DIR == nDimension)
            matKernel = (cv::Mat_<float> ( 1, 2 ) << -1, 1);
        else if (DIFF_ON_Y_DIR == nDimension)
            matKernel = (cv::Mat_<float> ( 2, 1 ) << -1, 1);

        cv::Mat matResult;
        cv::filter2D ( matInput, matResult, -1, matKernel, cv::Point ( -1, -1 ), 0.0, cv::BORDER_CONSTANT );
        if (DIFF_ON_X_DIR == nDimension)
            return cv::Mat ( matResult, cv::Rect ( 1, 0, matResult.cols - 1, matResult.rows ) );
        else if (DIFF_ON_Y_DIR == nDimension)
            return cv::Mat ( matResult, cv::Rect ( 0, 1, matResult.cols, matResult.rows - 1 ) );
        return cv::Mat ();
    }

    static double radian2Degree ( double dRadian );
    static double degree2Radian ( double dDegree );
    static float ptDisToLine ( const cv::Point2f &ptInput, bool bReversedFit, float fSlope, float fIntercept );
    static PR_Line2f calcEndPointOfLine ( const VectorOfPoint &vecPoint, bool bReversedFit, float fSlope, float fIntercept );
    static PR_Line2f calcEndPointOfLine ( const ListOfPoint &listPoint, bool bReversedFit, float fSlope, float fIntercept );
    static cv::Point2f lineIntersect ( float fSlope1, float fIntercept1, float fSlope2, float fIntercept2 );
    static float lineSlope ( const PR_Line2f &line );
    static void lineSlopeIntercept ( const PR_Line2f &line, float &fSlope, float &fIntercept );
    static VectorOfPoint getCornerOfRotatedRect ( const cv::RotatedRect &rotatedRect );
    static float guassianValue ( float ssq, float x );
    static cv::Mat generateGuassinDiffKernel ( int nOneSideWidth, float ssq );
    static void filter2D_Conv ( cv::InputArray src, cv::OutputArray dst, int ddepth,
        cv::InputArray kernel, cv::Point anchor = cv::Point ( -1, -1 ),
        double delta = 0, int borderType = cv::BORDER_DEFAULT );
    static float calcPointToContourDist ( const cv::Point &ptInput, const VectorOfPoint &contour, cv::Point &ptResult );
    static cv::Point2f getContourCtr ( const VectorOfPoint &contour );
    static int countOfNan ( const cv::Mat &matInput );
    static void findMinMaxCoord(const VectorOfPoint &vecPoints, int &xMin, int &xMax, int &yMin, int &yMax);
    static float calcFrequency(const cv::Mat &matInput);
    static VectorOfDouble interp1(const VectorOfDouble &vecX, const VectorOfDouble &vecV, const VectorOfDouble &vecXq, bool bSpine = false );
};

}
}
#endif