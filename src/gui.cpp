#include "gui.hpp"

using namespace std;
using namespace cv;

GUI::GUI()
{
    profile_picture = imread((char *)"media/ronnie.jpeg", 1);
    game_statistics.create(150, 300, CV_8UC3);
    profile_table.create(150, 1366, CV_8UC3);
    frame_count = 0;
}

GUI::~GUI()
{

} 

void GUI::ShowProfilePicture(){
    moveWindow("Profile Picture", 25, 50);
    // resizeWindow("Profile Picture", game_statistics.cols, game_statistics.rows);
    imshow("Profile Picture", profile_picture);
}

void GUI::ShowGameStatistics(string playername){
    game_statistics = Scalar(255,255,255);
    prepare_game_statistics(playername, game_statistics);
    moveWindow("Game Statistics",25, profile_picture.rows+100);
    imshow("Game Statistics", game_statistics);
}

void GUI::prepare_game_statistics(string playername, Mat game_statistics){
    
    stringstream ss;

    //Put rectangle as border to box
    rectangle(game_statistics, Point(0, 0), Point(game_statistics.cols-1, game_statistics.rows-1), Scalar(0,0,0), 5, 8);
    line(game_statistics, Point(0,game_statistics.rows/4), Point(game_statistics.cols,game_statistics.rows/4), Scalar(0, 0, 0), 1, CV_AA);
    line(game_statistics, Point(0,game_statistics.rows/2), Point(game_statistics.cols,game_statistics.rows/2), Scalar(0, 0, 0), 1, CV_AA);
    line(game_statistics, Point(0,game_statistics.rows/4*3), Point(game_statistics.cols,game_statistics.rows/4*3), Scalar(0, 0, 0), 1, CV_AA);
    line(game_statistics, Point(game_statistics.cols/2, 0), Point(game_statistics.cols/2,game_statistics.rows), Scalar(0, 0, 0), 1, CV_AA);

    putText(game_statistics, "Player", Point(10, game_statistics.rows/4-15), 1, 1, Scalar(0, 0, 0), 2);
    putText(game_statistics, playername, Point(game_statistics.cols/2+10, game_statistics.rows/4-15), 1, 1, Scalar(0, 0, 0), 2);
    // putText(game_statistics, username, Point(game_statistics.cols/2+10, game_statistics.rows/4-15), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0), 2);
    putText(game_statistics, "Frame", Point(10, game_statistics.rows/2-15), 1, 1, Scalar(0, 0, 0), 2);
	ss << " " << frame_count;
    putText(game_statistics, ss.str(), Point(game_statistics.cols/2+10, game_statistics.rows/2-15), 1, 1, Scalar(0, 0, 0), 1);

    putText(game_statistics, "Shot", Point(10, game_statistics.rows/4*3-15), 1, 1, Scalar(0, 0, 0), 2);
    ss.str(" ");
	ss << " " << shot_count;
    putText(game_statistics, ss.str(), Point(game_statistics.cols/2+10, game_statistics.rows/4*3-15), 1, 1, Scalar(0, 0, 0), 2);

    putText(game_statistics, "Match", Point(10, game_statistics.rows-15), 1, 1, Scalar(0, 0, 0), 2);
    putText(game_statistics, "1", Point(game_statistics.cols/2+10, game_statistics.rows-15), 1, 1, Scalar(0, 0, 0), 2);
}

void GUI::ShowProfileTable(){
    profile_table = Scalar(255,255,255);
    prepare_profile_table();
    moveWindow("Profile Table",25, 550);
    imshow("Profile Table", profile_table);
}
void GUI::prepare_profile_table()
{
     string skills[] = {"Played", "Won","Lost","Played", "Won","Lost","Straight", "Cut", "Safety",
        "Spin Acc.", "Power", "Max Score", "Risk", "Overall", "Match", "Frame", "Shot Accuracy"};
    rectangle(profile_table, Point(0, 0), Point(profile_table.cols-1, profile_table.rows-1), Scalar(0,0,0), 5, 8);
    line(profile_table, Point(0, profile_table.rows/3), Point(profile_table.cols/14*9,profile_table.rows/3), Scalar(0, 0, 0), 1, CV_AA);
    line(profile_table, Point(0, profile_table.rows/3*2), Point(profile_table.cols,profile_table.rows/3*2), Scalar(0, 0, 0), 1, CV_AA);

    for(int i=0;i<14;i++){
        if(i==0 || i==1 || i==3 || i==4 || i==6 || i==7)
            line(profile_table, Point(profile_table.cols/14*(i+1), profile_table.rows/3), Point(profile_table.cols/14*(i+1),profile_table.rows), Scalar(0, 0, 0), 1, CV_AA);
        else
            line(profile_table, Point(profile_table.cols/14*(i+1), 0), Point(profile_table.cols/14*(i+1),profile_table.rows), Scalar(0, 0, 0), 1, CV_AA);
        putText(profile_table, skills[i], Point((profile_table.cols/14*(i))+10,profile_table.rows/3*2-20), 1, 1, Scalar(0, 0, 0), 2);
        if(i<6)
            putText(profile_table, "1", Point((profile_table.cols/14*(i))+10,profile_table.rows-20), 1, 1, Scalar(0, 0, 0), 2);
    }
    
    putText(profile_table, skills[14], Point((profile_table.cols/14)+10,profile_table.rows/3-20), 1, 1, Scalar(0, 0, 0), 2);
    putText(profile_table, skills[15], Point((profile_table.cols/14*4)+10,profile_table.rows/3-20), 1, 1, Scalar(0, 0, 0), 2);
    putText(profile_table, skills[16], Point((profile_table.cols/14*7)-10,profile_table.rows/3-20), 1, 1, Scalar(0, 0, 0), 2);
}
