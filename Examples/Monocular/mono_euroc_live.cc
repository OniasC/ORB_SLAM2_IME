/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<iomanip>
#include<signal.h>

#include<opencv2/core/core.hpp>

#include<System.h>

using namespace std;

volatile sig_atomic_t flag = 0;
void my_function(int sig) {flag = 1;}

#ifdef COMPILEDWITHC11
#define SET_CLOCK(t0) \
  std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();

#else

#define SET_CLOCK(t0) \
  std::chrono::monotonic_clock::time_point t0 = std::chrono::monotonic_clock::now();
#endif
#define TIME_DIFF(t0, t1) \
  (std::chrono::duration_cast<std::chrono::duration<double>>((t1) - (t0)).count())

int main(int argc, char **argv)
{
    signal(SIGINT, my_function);

    if(argc != 3)
    {
        cerr << endl << "Usage: ./mono_euroc path_to_vocabulary path_to_settings" << endl;
        return 1;
    }
    
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        cerr << endl  <<"Could not open camera feed."  << endl;
        return -1;
    }
    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true);

    int number_of_frames = 0;
    SET_CLOCK(t0);
    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;

    // Main loop
    double tframe;
    while(1)
    {
        if(flag){
            flag = 0;
            cout << "\n ========================= \n exiting while loop \n ===================== \n";
            break;
        }

        //Create a new Mat
        cv::Mat frame;
        //Send the captured frame to the new Mat
        cap>>frame;

        SET_CLOCK(t1);
        tframe = TIME_DIFF(t0,t1);
        // Pass the image to the SLAM system
        SLAM.TrackMonocular(frame, tframe);
        number_of_frames++;
    }

    // Stop all threads
    SLAM.Shutdown();
    // Statics:
    cout<<"Mean frames per second:"<<number_of_frames/tframe<<endl;
    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
    // save the pointcloud
    SLAM.CreatePCD("pointcloud.pcd");

    return 0;
}