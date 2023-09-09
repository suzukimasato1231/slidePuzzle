#pragma once
#include<string>
#define MAP_WIDTH (20) //横の最大枚数
#define MAP_HEIGHT (20)  //縦の最大枚数
#define MAP_ (100)  //縦の最大枚数
#include<Vec.h>
/// <summary>
/// CSVファイルの読み込み
/// </summary>
/// <param name="map"> 読み込んだCSVファイルの保存先 </param>
/// <param name="FilePath"> CSVファイルのパス </param>
/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
/// <returns> 0以外で失敗、0で成功 </returns>
int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const std::string& FilePath,const int width,const int height);

/// <summary>
/// CSVファイルの読み込み(1次元配列)
/// </summary>
/// <param name="mapArray"> 読み込んだCSVファイルの保存先 </param>
/// <param name="FilePath"> CSVファイルのパス </param>
/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
/// <returns> 0以外で失敗、0で成功 </returns>
int LoadCSV1D(int *mapArray, const size_t &mapSize, const std::string& FilePath, int LoadStopNumber = -1);

/// <summary>
/// サイズを調べる
/// </summary>
/// <param name="width">横サイズ入れる</param>
/// <param name="height">縦サイズを入れる</param>
void LoadSize(int &width,int &height, const std::string& FilePath);