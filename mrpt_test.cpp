
/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2019, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */

/**
 * Execute the iterating closest point algorithm (ICP) on a hardcoded pair of
 * laser data. The algorithm computes the transformation (translation and
 * rotation) for aligning the 2 sets of laser scans and plots the
 */

// FIXME: was essential for <mrpt/slam.h>
// #include <cstddef>
// #include <mrpt/core/common.h>

#include <mrpt/gui.h>
#include <mrpt/maps/CSimplePointsMap.h>
#include <mrpt/math/utils.h>
#include <mrpt/obs/CObservation2DRangeScan.h>
#include <mrpt/obs/stock_observations.h>
#include <mrpt/poses/CPose2D.h>
#include <mrpt/poses/CPosePDF.h>
#include <mrpt/poses/CPosePDFGaussian.h>
#include <mrpt/slam/CICP.h>

#include <opencv2/highgui/highgui.hpp>

#include <fstream>
#include <iostream>

using namespace mrpt;
using namespace mrpt::slam;
using namespace mrpt::maps;
using namespace mrpt::obs;
using namespace mrpt::math;
using namespace mrpt::poses;
using namespace std;
using namespace cv;

bool skip_window = false;
int ICP_method = (int) icpClassic;

// ------------------------------------------------------
//				TestICP
// ------------------------------------------------------

float randNumber(float Min, float Max) {
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

struct InputData {
    CSimplePointsMap m1, m2;

    CPose2D initialPose{0.0f, 0.0f, (float) DEG2RAD(0.0f)};
};

// ./libs/obs/src/stock_observations.cpp
void TestICP(InputData &data) {
//    CSimplePointsMap m1, m2;
    float runningTime;
    CICP::TReturnInfo info;
    CICP ICP;

//    // Load scans:
//    CObservation2DRangeScan scan1;
//    stock_observations::example2DRangeScan(scan1, 0);
//
//    CObservation2DRangeScan scan2;
//    stock_observations::example2DRangeScan(scan2, 1);

    // Build the points maps from the scans:
    // ./libs/obs/src/CMetricMap.cpp
//	m1.insertObservation(&scan1);
//	m2.insertObservation(&scan2);

    // ./libs/maps/src/maps/CSimplePointsMap.cpp
//    m1.loadFromRangeScan(scan1);
//    m2.loadFromRangeScan(scan2);

#if MRPT_HAS_PCL
    cout << "Saving map1.pcd and map2.pcd in PCL format...\n";
    m1.savePCDFile("map1.pcd", false);
    m2.savePCDFile("map2.pcd", false);
#endif

    // -----------------------------------------------------

    /**
     * user-set parameters for the icp algorithm.
     * For a full list of the available options check the online tutorials
     * page:
     * https://www.mrpt.org/Iterative_Closest_Point_(ICP)_and_other_matching_algorithms
     */

    //	select which algorithm version to use
    //	ICP.options.ICP_algorithm = icpLevenbergMarquardt;
    //	ICP.options.ICP_algorithm = icpClassic;
    ICP.options.ICP_algorithm = (TICPAlgorithm) ICP_method;

    // configuration options for the icp algorithm
    ICP.options.maxIterations = 100;
    ICP.options.thresholdAng = DEG2RAD(2.0f);
    ICP.options.thresholdDist = 0.75f;
    ICP.options.ALFA = 0.5f;
    ICP.options.smallestThresholdDist = 0.05f;
    ICP.options.doRANSAC = false;
//    ICP.options.corresponding_points_decimation = 1;

    ICP.options.dumpToConsole();
    // -----------------------------------------------------

    /**
     * Scans alignment procedure.
     * Given an initial guess (initialPose) and the maps to be aligned, the
     * algorithm returns the probability density function (pdf) of the alignment
     * Additional arguments are provided to investigate the performance of the
     * algorithm
     */

    CPosePDF::Ptr pdf =
            ICP.Align(&data.m1, &data.m2, data.initialPose, &runningTime, (void *) &info);

    printf(
            "ICP run in %.02fms, %d iterations (%.02fms/iter), %.01f%% goodness\n "
            "-> ",
            runningTime * 1000, info.nIterations,
            runningTime * 1000.0f / info.nIterations, info.goodness * 100);

    cout << "Mean of estimation: " << pdf->getMeanVal() << endl << endl;

    CPosePDFGaussian gPdf;
    gPdf.copyFrom(*pdf);

    cout << "Covariance of estimation: " << endl << gPdf.cov << endl;

    cout << " std(x): " << sqrt(gPdf.cov(0, 0)) << endl;
    cout << " std(y): " << sqrt(gPdf.cov(1, 1)) << endl;
    cout << " std(phi): " << RAD2DEG(sqrt(gPdf.cov(2, 2))) << " (deg)" << endl;

    // cout << "Covariance of estimation (MATLAB format): " << endl <<
    // gPdf.cov.inMatlabFormat()  << endl;

    /**
     * Save the results for potential postprocessing (in txt and in matlab
     * format)
     */
    cout << "-> Saving reference map as scan1.txt" << endl;
    data.m1.save2D_to_text_file("scan1.txt");

    cout << "-> Saving map to align as scan2.txt" << endl;
    data.m2.save2D_to_text_file("scan2.txt");

    cout << "-> Saving transformed map to align as scan2_trans.txt" << endl;
    CSimplePointsMap m2_trans = data.m2;
    m2_trans.changeCoordinatesReference(gPdf.mean);
    m2_trans.save2D_to_text_file("scan2_trans.txt");

    cout << "-> Saving MATLAB script for drawing 2D ellipsoid as view_ellip.m"
         << endl;
    CMatrixFloat COV22 = CMatrixFloat(CMatrixDouble(gPdf.cov));
    COV22.setSize(2, 2);
    CVectorFloat MEAN2D(2);
    MEAN2D[0] = gPdf.mean.x();
    MEAN2D[1] = gPdf.mean.y();
    {
        ofstream f("view_ellip.m");
        f << math::MATLAB_plotCovariance2D(COV22, MEAN2D, 3.0f);
    }

// If we have 2D windows, use'em:
#if MRPT_HAS_WXWIDGETS
    /**
     * Plotting the icp results:
     * Aligned maps + transformation uncertainty ellipsis
     */
    if (!skip_window) {
        gui::CDisplayWindowPlots win("ICP results");

        // Reference map:
        vector<float> map1_xs, map1_ys, map1_zs;
        data.m1.getAllPoints(map1_xs, map1_ys, map1_zs);
        win.plot(map1_xs, map1_ys, "b.3", "map1");

        data.m2.getAllPoints(map1_xs, map1_ys, map1_zs);
        win.plot(map1_xs, map1_ys, "g.5", "map3");

        // Translated map:
        vector<float> map2_xs, map2_ys, map2_zs;
        m2_trans.getAllPoints(map2_xs, map2_ys, map2_zs);
        win.plot(map2_xs, map2_ys, "r.3", "map2");

        // Uncertainty
        win.plotEllipse(MEAN2D[0], MEAN2D[1], COV22, 3.0, "b2", "cov");

        win.axis(-1, 10, -16, 16);
        win.axis_equal();

        cout << "Close the window to exit" << endl;
        win.waitForKey();
    }
#endif
}

template<typename T, typename U>
U clamp(U x, T mnx, T mxx) {
    if (x > mxx) {
        x = mxx;
    }
    if (x < mnx) {
        x = mnx;
    }
    return x;
}

int main(int argc, char **argv) {

    // Load own map
    Mat raw_map = imread("../laser_hm.png", 0);
    Mat z = Mat::zeros(raw_map.size(), CV_8U);

    const float cell_size_m = 0.01;
    const float ycell_size_m = 0.05;
    const float dy = 3;
    const float big_cell_size_m = 6;
    cout << "dX:" << raw_map.cols * cell_size_m << endl;

    vector<Point2i> hits;

    vector<CSimplePointsMap> tails;
    float dist_from_zero = 0;
    CSimplePointsMap current_map;

    CSimplePointsMap target_map;
    bool fill_first = true;
    for (int col = 0; col < raw_map.cols; ++col) {
        dist_from_zero += cell_size_m;
        for (int row = 0; row < raw_map.rows; ++row) {
            uint8_t pxl = raw_map.at<uint8_t>(row, col);
            if (pxl < 128) {
                int x = col;
                int y = row;
                y = clamp(y + randNumber(-dy, +dy), 0, raw_map.cols);

                z.at<uint8_t>(y, x) = 255;

                current_map.insertPointFast(x * cell_size_m, -y * ycell_size_m);

                if (fill_first && dist_from_zero < 3 && col % 3 == 0) {
                    int y1 = clamp(y + randNumber(-dy, +dy), 0, raw_map.cols);
                    target_map.insertPointFast(x * cell_size_m, -y1 * ycell_size_m + 2);
                }
                hits.push_back(Point2i{x, y});
                break;
            }
        }

        if (dist_from_zero >= big_cell_size_m) {
            dist_from_zero = 0;
            tails.push_back(current_map);
            current_map.clear();
            fill_first = false;
            break;
        }
    }
    imwrite("../whole_map.png", z);

//    if (!skip_window) {
//        gui::CDisplayWindowPlots win("ICP results");
//
//        // Reference map:
//        int ptr = 0;
//        for (auto &tail : tails) {
//            vector<float> map1_xs, map1_ys, map1_zs;
//            tail.getAllPoints(map1_xs, map1_ys, map1_zs);
//            if (ptr % 2 == 0) {
//                win.plot(map1_xs, map1_ys, "b.3", "map" + to_string(ptr));
//            } else {
//                win.plot(map1_xs, map1_ys, "r.3", "map" + to_string(ptr));
//            }
//            ++ptr;
//        }
//
//        win.axis(-1, 10, -6, 6);
//        win.axis_equal();
//
//        cout << "Close the window to exit" << endl;
//        win.waitForKey();
//    }
//
//    return 1;

    try {
        skip_window = (argc > 2);
        if (argc > 1) {
            ICP_method = atoi(argv[1]);
        }

        InputData data;
        data.m1 = tails.front();
        data.m2 = target_map;
        // FIXME: Attention!!! Very important!
        data.initialPose = CPose2D{0.0f, -0.9f, (float) DEG2RAD(0.0f)};
//        for (int i = 0; i < 90; ++i) {
//            float shift = 2;
//            double rv = randNumber(-0.05, 0.05);
//
//            if (i > 50) {
//                shift = 0;
//            }
//
//            if (i > 70) {
//                shift = (i - 70) * 0.3;
//            }
//
//            if (i > 0) {
//                data.m1.insertPointFast(rv + shift, (i) / 10.0);
//            }
//            data.m2.insertPointFast(rv + shift, (i) / 10.0 + 2);
//        }

        TestICP(data);

//        if (!skip_window) {
//            gui::CDisplayWindowPlots win("ICP results");
//
//            // Reference map:
//            vector<float> map1_xs, map1_ys, map1_zs;
//            data.m1.getAllPoints(map1_xs, map1_ys, map1_zs);
//            win.plot(map1_xs, map1_ys, "b.3", "map1");
//
//            win.axis(-1, 10, -6, 6);
//            win.axis_equal();
//
//            cout << "Close the window to exit" << endl;
//            win.waitForKey();
//        }

        return 0;
    }
    catch (exception &e) {
        cout << "MRPT exception caught: " << e.what() << endl;
        return -1;
    }
    catch (...) {
        printf("Another exception!!");
        return -1;
    }
}