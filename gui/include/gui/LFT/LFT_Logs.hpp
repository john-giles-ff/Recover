#ifndef LFT_LOGS_HPP
#define LFT_LOGS_HPP

#include <gui\model\Model.hpp>
#include <gui\LFT\LFT_Information.hpp>
#include <gui\LFT\Definitions\RecoverLog.hpp>
#include <algorithm>

//const short SAMPLE_BUFFER_SIZE = 5;
const short LOG_MAX = 64;
//const short SAMPLE_MAX = 1000;


class LFT_Logs
{
public:	
	LFT_Logs(LFT_Information * information);

	void SetModel(Model* model);

	RecoverLog GetHeader(int index);

	bool IsNextLogPresent();
	
	
	void QueGetAllSamples(int logIndex, int sampleCount);
	void GetAllSamples(int logIndex = -1, int sampleCount = -1);
	void AbortSampleLoading();
	void DeleteLog(int logIndex);

	void QueGetAllHeaders();
	void GetAllHeaders();

	void InsertionSort();

	void QueExportAll();
	void ExportAll();

	RecoverLog* GetSampleQueLogs();
	ThreadSafe<bool> GetSamplesRequired = false;
	ThreadSafe<bool> GetLogsRequired = false;
	ThreadSafe<bool> ExportAllRequired = false;
	ThreadSafe<int> ExportLogsResult = 0;
	ThreadSafe<int> GetLogsProgress = 0;
	
	RecoverLog LoadedLogs[LOG_MAX];
#ifndef SIMULATOR
	Sample LoadedSamples[SAMPLE_MAX];
#else
	Sample LoadedSamples[SAMPLE_MAX] = 
	{		
		Sample(SAMPLE_INITIALISE, 0, 30, 22, 2046, 1234),
		Sample(SAMPLE_INITIALISE, 1, 30, 22, 2046, 1234),
		Sample(SAMPLE_INITIALISE, 2, 31, 23, 2046, 1234),
		Sample(SAMPLE_INITIALISE, 3, 32, 23, 2046, 1234),
		Sample(SAMPLE_INITIALISE, 4, 32, 23, 2046, 1234),
		Sample(SAMPLE_INITIALISE, 5, 32, 23, 2046, 1234),
		Sample(SAMPLE_INITIALISE, 6, 33, 24, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 7, 32, 24, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 8, 32, 24, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 9, 33, 24, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 10, 33, 25, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 11, 33, 25, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 12, 33, 25, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 13, 33, 26, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 14, 33, 26, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 15, 33, 26, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 16, 33, 27, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 17, 34, 27, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 18, 34, 27, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 19, 34, 27, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 20, 34, 28, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 21, 34, 28, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 22, 34, 28, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 23, 34, 28, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 24, 34, 28, 2046, 1234),
		Sample(SAMPLE_PUMPDOWN, 25, 34, 29, 1922, 1234),
		Sample(SAMPLE_PUMPDOWN, 26, 34, 29, 1746, 1234),
		Sample(SAMPLE_PUMPDOWN, 27, 34, 29, 1597, 1234),
		Sample(SAMPLE_PUMPDOWN, 28, 34, 29, 1500, 1234),
		Sample(SAMPLE_PUMPDOWN, 29, 34, 30, 1331, 1234),
		Sample(SAMPLE_PUMPDOWN, 30, 34, 30, 1183, 1234),
		Sample(SAMPLE_PUMPDOWN, 31, 34, 30, 1090, 1234),
		Sample(SAMPLE_PUMPDOWN, 32, 34, 30, 979, 1234),
		Sample(SAMPLE_PUMPDOWN, 33, 34, 31, 907, 1234),
		Sample(SAMPLE_PUMPDOWN, 34, 35, 31, 824, 1234),
		Sample(SAMPLE_PUMPDOWN, 35, 35, 31, 769, 1234),
		Sample(SAMPLE_PUMPDOWN, 36, 35, 31, 706, 1234),
		Sample(SAMPLE_PUMPDOWN, 37, 34, 31, 650, 1234),
		Sample(SAMPLE_PUMPDOWN, 38, 35, 31, 612, 1234),
		Sample(SAMPLE_PUMPDOWN, 39, 34, 31, 570, 1234),
		Sample(SAMPLE_PUMPDOWN, 40, 34, 31, 539, 1234),
		Sample(SAMPLE_PUMPDOWN, 41, 35, 31, 503, 1234),
		Sample(SAMPLE_PUMPDOWN, 42, 35, 31, 478, 1234),
		Sample(SAMPLE_PUMPDOWN, 43, 35, 31, 448, 1234),
		Sample(SAMPLE_PUMPDOWN, 44, 34, 32, 427, 1234),
		Sample(SAMPLE_PUMPDOWN, 45, 35, 32, 403, 1234),
		Sample(SAMPLE_PUMPDOWN, 46, 35, 32, 381, 1234),
		Sample(SAMPLE_PUMPDOWN, 47, 34, 32, 366, 1234),
		Sample(SAMPLE_PUMPDOWN, 48, 34, 32, 348, 1234),
		Sample(SAMPLE_PUMPDOWN, 49, 35, 32, 335, 1234),
		Sample(SAMPLE_PUMPDOWN, 50, 35, 32, 319, 1234),
		Sample(SAMPLE_PUMPDOWN, 51, 35, 32, 304, 1234),
		Sample(SAMPLE_PUMPDOWN, 52, 35, 32, 294, 1234),
		Sample(SAMPLE_PUMPDOWN, 53, 35, 32, 282, 1234),
		Sample(SAMPLE_PUMPDOWN, 54, 34, 32, 270, 1234),
		Sample(SAMPLE_PUMPDOWN, 55, 35, 32, 262, 1234),
		Sample(SAMPLE_PUMPDOWN, 56, 35, 32, 252, 1234),
		Sample(SAMPLE_PUMPDOWN, 57, 35, 32, 244, 1234),
		Sample(SAMPLE_PUMPDOWN, 58, 35, 32, 236, 1234),
		Sample(SAMPLE_PUMPDOWN, 59, 35, 32, 227, 1234),
		Sample(SAMPLE_PUMPDOWN, 60, 35, 32, 221, 1234),
		Sample(SAMPLE_PUMPDOWN, 61, 35, 32, 214, 1234),
		Sample(SAMPLE_PUMPDOWN, 62, 35, 32, 208, 1234),
		Sample(SAMPLE_PUMPDOWN, 63, 35, 32, 201, 1234),
		Sample(SAMPLE_PUMPDOWN, 64, 35, 32, 195, 1234),
		Sample(SAMPLE_PUMPDOWN, 65, 35, 32, 191, 1234),
		Sample(SAMPLE_PUMPDOWN, 66, 35, 32, 185, 1234),
		Sample(SAMPLE_PUMPDOWN, 67, 34, 32, 181, 1234),
		Sample(SAMPLE_PUMPDOWN, 68, 35, 32, 176, 1234),
		Sample(SAMPLE_PUMPDOWN, 69, 35, 32, 171, 1234),
		Sample(SAMPLE_PUMPDOWN, 70, 34, 32, 167, 1234),
		Sample(SAMPLE_PUMPDOWN, 71, 35, 32, 163, 1234),
		Sample(SAMPLE_PUMPDOWN, 72, 35, 32, 160, 1234),
		Sample(SAMPLE_PUMPDOWN, 73, 35, 32, 156, 1234),
		Sample(SAMPLE_PUMPDOWN, 74, 34, 32, 152, 1234),
		Sample(SAMPLE_PUMPDOWN, 75, 34, 33, 165, 1234),
		Sample(SAMPLE_PUMPDOWN, 76, 35, 33, 147, 1234),
		Sample(SAMPLE_PUMPDOWN, 77, 35, 32, 144, 1234),
		Sample(SAMPLE_PUMPDOWN, 78, 35, 33, 141, 1234),
		Sample(SAMPLE_PUMPDOWN, 79, 35, 33, 138, 1234),
		Sample(SAMPLE_PUMPDOWN, 80, 35, 33, 135, 1234),
		Sample(SAMPLE_PUMPDOWN, 81, 35, 33, 133, 1234),
		Sample(SAMPLE_PUMPDOWN, 82, 35, 33, 130, 1234),
		Sample(SAMPLE_PUMPDOWN, 83, 34, 33, 128, 1234),
		Sample(SAMPLE_PUMPDOWN, 84, 35, 33, 125, 1234),
		Sample(SAMPLE_PUMPDOWN, 85, 35, 33, 123, 1234),
		Sample(SAMPLE_PUMPDOWN, 86, 35, 33, 121, 1234),
		Sample(SAMPLE_PUMPDOWN, 87, 35, 33, 118, 1234),
		Sample(SAMPLE_PUMPDOWN, 88, 35, 33, 116, 1234),
		Sample(SAMPLE_PUMPDOWN, 89, 35, 33, 114, 1234),
		Sample(SAMPLE_PUMPDOWN, 90, 35, 33, 112, 1234),
		Sample(SAMPLE_PUMPDOWN, 91, 34, 33, 111, 1234),
		Sample(SAMPLE_PUMPDOWN, 92, 34, 33, 109, 1234),
		Sample(SAMPLE_PUMPDOWN, 93, 35, 33, 107, 1234),
		Sample(SAMPLE_PUMPDOWN, 94, 35, 33, 106, 1234),
		Sample(SAMPLE_PUMPDOWN, 95, 35, 33, 104, 1234),
		Sample(SAMPLE_PUMPDOWN, 96, 35, 33, 103, 1234),
		Sample(SAMPLE_PUMPDOWN, 97, 35, 33, 101, 1234),
		Sample(SAMPLE_PUMPDOWN, 98, 34, 33, 99, 1234),
		Sample(SAMPLE_PUMPDOWN, 99, 35, 33, 98, 1234),
		Sample(SAMPLE_PUMPDOWN, 100, 34, 33, 97, 1234),
		Sample(SAMPLE_PUMPDOWN, 101, 34, 33, 96, 1234),
		Sample(SAMPLE_PUMPDOWN, 102, 35, 33, 94, 1234),
		Sample(SAMPLE_PUMPDOWN, 103, 34, 33, 93, 1234),
		Sample(SAMPLE_PUMPDOWN, 104, 35, 33, 92, 1234),
		Sample(SAMPLE_PUMPDOWN, 105, 35, 33, 90, 1234),
		Sample(SAMPLE_PUMPDOWN, 106, 34, 33, 89, 1234),
		Sample(SAMPLE_PUMPDOWN, 107, 35, 33, 88, 1234),
		Sample(SAMPLE_PUMPDOWN, 108, 35, 33, 87, 1234),
		Sample(SAMPLE_PUMPDOWN, 109, 34, 33, 86, 1234),
		Sample(SAMPLE_PUMPDOWN, 110, 35, 33, 85, 1234),
		Sample(SAMPLE_PUMPDOWN, 111, 35, 33, 84, 1234),
		Sample(SAMPLE_PUMPDOWN, 112, 34, 33, 83, 1234),
		Sample(SAMPLE_PUMPDOWN, 113, 35, 33, 82, 1234),
		Sample(SAMPLE_PUMPDOWN, 114, 35, 33, 81, 1234),
		Sample(SAMPLE_PUMPDOWN, 115, 35, 33, 80, 1234),
		Sample(SAMPLE_PUMPDOWN, 116, 35, 33, 79, 1234),
		Sample(SAMPLE_PUMPDOWN, 117, 35, 33, 78, 1234),
		Sample(SAMPLE_PUMPDOWN, 118, 34, 33, 77, 1234),
		Sample(SAMPLE_PUMPDOWN, 119, 34, 33, 77, 1234),
		Sample(SAMPLE_PUMPDOWN, 120, 34, 33, 76, 1234),
		Sample(SAMPLE_PUMPDOWN, 121, 34, 33, 75, 1234),
		Sample(SAMPLE_PUMPDOWN, 122, 34, 33, 74, 1234),
		Sample(SAMPLE_PUMPDOWN, 123, 35, 33, 74, 1234),
		Sample(SAMPLE_PUMPDOWN, 124, 35, 33, 73, 1234),
		Sample(SAMPLE_PUMPDOWN, 125, 35, 33, 72, 1234),
		Sample(SAMPLE_PUMPDOWN, 126, 35, 33, 72, 1234),
		Sample(SAMPLE_PUMPDOWN, 127, 35, 33, 71, 1234),
		Sample(SAMPLE_PUMPDOWN, 128, 35, 33, 70, 1234),
		Sample(SAMPLE_PUMPDOWN, 129, 34, 33, 70, 1234),
		Sample(SAMPLE_PUMPDOWN, 130, 34, 33, 69, 1234),
		Sample(SAMPLE_PUMPDOWN, 131, 35, 33, 68, 1234),
		Sample(SAMPLE_PUMPDOWN, 132, 35, 33, 68, 1234),
		Sample(SAMPLE_PUMPDOWN, 133, 34, 33, 67, 1234),
		Sample(SAMPLE_PUMPDOWN, 134, 35, 33, 67, 1234),
		Sample(SAMPLE_PUMPDOWN, 135, 34, 33, 66, 1234),
		Sample(SAMPLE_PUMPDOWN, 136, 35, 33, 65, 1234),
		Sample(SAMPLE_PUMPDOWN, 137, 34, 33, 65, 1234),
		Sample(SAMPLE_PUMPDOWN, 138, 35, 33, 64, 1234),
		Sample(SAMPLE_PUMPDOWN, 139, 35, 33, 64, 1234),
		Sample(SAMPLE_PUMPDOWN, 140, 35, 33, 63, 1234),
		Sample(SAMPLE_PUMPDOWN, 141, 34, 33, 63, 1234),
		Sample(SAMPLE_PUMPDOWN, 142, 35, 33, 63, 1234),
		Sample(SAMPLE_PUMPDOWN, 143, 35, 33, 62, 1234),
		Sample(SAMPLE_PUMPDOWN, 144, 34, 33, 61, 1234),
		Sample(SAMPLE_PUMPDOWN, 145, 35, 33, 61, 1234),
		Sample(SAMPLE_PUMPDOWN, 146, 34, 33, 60, 1234),
		Sample(SAMPLE_PUMPDOWN, 147, 35, 33, 60, 1234),
		Sample(SAMPLE_PUMPDOWN, 148, 35, 33, 60, 1234),
		Sample(SAMPLE_PUMPDOWN, 149, 34, 33, 59, 1234),
		Sample(SAMPLE_PUMPDOWN, 150, 34, 33, 59, 1234),
		Sample(SAMPLE_PUMPDOWN, 151, 35, 33, 59, 1234),
		Sample(SAMPLE_PUMPDOWN, 152, 35, 33, 58, 1234),
		Sample(SAMPLE_PUMPDOWN, 153, 34, 33, 58, 1234),
		Sample(SAMPLE_PUMPDOWN, 154, 35, 33, 57, 1234),
		Sample(SAMPLE_PUMPDOWN, 155, 35, 33, 57, 1234),
		Sample(SAMPLE_PUMPDOWN, 156, 35, 33, 57, 1234),
		Sample(SAMPLE_PUMPDOWN, 157, 35, 33, 56, 1234),
		Sample(SAMPLE_PUMPDOWN, 158, 34, 33, 56, 1234),
		Sample(SAMPLE_PUMPDOWN, 159, 34, 33, 56, 1234),
		Sample(SAMPLE_PUMPDOWN, 160, 35, 33, 55, 1234),
		Sample(SAMPLE_PUMPDOWN, 161, 35, 33, 55, 1234),
		Sample(SAMPLE_PUMPDOWN, 162, 34, 33, 54, 1234),
		Sample(SAMPLE_PUMPDOWN, 163, 35, 33, 54, 1234),
		Sample(SAMPLE_PUMPDOWN, 164, 34, 33, 54, 1234),
		Sample(SAMPLE_PUMPDOWN, 165, 35, 33, 54, 1234),
		Sample(SAMPLE_PUMPDOWN, 166, 35, 33, 53, 1234),
		Sample(SAMPLE_PUMPDOWN, 167, 34, 33, 53, 1234),
		Sample(SAMPLE_PUMPDOWN, 168, 34, 33, 53, 1234),
		Sample(SAMPLE_PUMPDOWN, 169, 35, 33, 52, 1234),
		Sample(SAMPLE_PUMPDOWN, 170, 34, 33, 52, 1234),
		Sample(SAMPLE_PUMPDOWN, 171, 35, 33, 52, 1234),
		Sample(SAMPLE_PUMPDOWN, 172, 34, 33, 52, 1234),
		Sample(SAMPLE_PUMPDOWN, 173, 35, 33, 51, 1234),
		Sample(SAMPLE_PUMPDOWN, 174, 34, 33, 51, 1234),
		Sample(SAMPLE_PUMPDOWN, 175, 34, 33, 51, 1234),
		Sample(SAMPLE_PUMPDOWN, 176, 34, 33, 51, 1234),
		Sample(SAMPLE_PUMPDOWN, 177, 34, 33, 50, 1234),
		Sample(SAMPLE_PUMPDOWN, 178, 34, 33, 50, 1234),
		Sample(SAMPLE_PUMPDOWN, 179, 34, 33, 50, 1234),
		Sample(SAMPLE_PUMPDOWN, 180, 35, 33, 49, 1234),
		Sample(SAMPLE_PUMPDOWN, 181, 35, 33, 49, 1234),
		Sample(SAMPLE_PUMPDOWN, 182, 35, 33, 49, 1234),
		Sample(SAMPLE_PUMPDOWN, 183, 35, 33, 49, 1234),
		Sample(SAMPLE_PUMPDOWN, 184, 35, 33, 49, 1234),
		Sample(SAMPLE_PUMPDOWN, 185, 34, 33, 48, 1234),
		Sample(SAMPLE_PUMPDOWN, 186, 35, 33, 48, 1234),
		Sample(SAMPLE_PUMPDOWN, 187, 35, 33, 48, 1234),
		Sample(SAMPLE_PUMPDOWN, 188, 35, 33, 48, 1234),
		Sample(SAMPLE_PUMPDOWN, 189, 35, 33, 47, 1234),
		Sample(SAMPLE_PUMPDOWN, 190, 34, 33, 47, 1234),
		Sample(SAMPLE_PUMPDOWN, 191, 35, 33, 47, 1234),
		Sample(SAMPLE_PUMPDOWN, 192, 34, 33, 47, 1234),
		Sample(SAMPLE_PUMPDOWN, 193, 35, 33, 47, 1234),
		Sample(SAMPLE_PUMPDOWN, 194, 34, 33, 46, 1234),
		Sample(SAMPLE_PUMPDOWN, 195, 35, 33, 46, 1234),
		Sample(SAMPLE_HEAT, 196, 34, 38, 46, 1234),
		Sample(SAMPLE_HEAT, 197, 34, 77, 51, 1234),
		Sample(SAMPLE_HEAT, 198, 35, 108, 52, 1234),
		Sample(SAMPLE_HEAT, 199, 35, 135, 53, 1234),
		Sample(SAMPLE_HEAT, 200, 35, 159, 55, 1234),
		Sample(SAMPLE_HEAT, 201, 35, 179, 56, 1234),
		Sample(SAMPLE_HEAT, 202, 35, 184, 57, 1234),
		Sample(SAMPLE_HEAT, 203, 35, 190, 58, 1234),
		Sample(SAMPLE_HEAT, 204, 35, 190, 58, 1234),
		Sample(SAMPLE_HEAT, 205, 35, 189, 59, 1234),
		Sample(SAMPLE_HEAT, 206, 35, 189, 59, 1234),
		Sample(SAMPLE_HEAT, 207, 35, 190, 60, 1234),
		Sample(SAMPLE_HEAT, 208, 35, 189, 60, 1234),
		Sample(SAMPLE_HEAT, 209, 35, 189, 61, 1234),
		Sample(SAMPLE_HEAT, 210, 35, 190, 61, 1234),
		Sample(SAMPLE_HEAT, 211, 35, 189, 61, 1234),
		Sample(SAMPLE_HEAT, 212, 36, 189, 62, 1234),
		Sample(SAMPLE_HEAT, 213, 36, 189, 62, 1234),
		Sample(SAMPLE_HEAT, 214, 36, 189, 62, 1234),
		Sample(SAMPLE_HEAT, 215, 36, 189, 63, 1234),
		Sample(SAMPLE_HEAT, 216, 36, 190, 63, 1234),
		Sample(SAMPLE_HEAT, 217, 36, 189, 64, 1234),
		Sample(SAMPLE_HEAT, 218, 36, 189, 64, 1234),
		Sample(SAMPLE_HEAT, 219, 36, 189, 64, 1234),
		Sample(SAMPLE_HEAT, 220, 36, 189, 65, 1234),
		Sample(SAMPLE_HEAT, 221, 37, 190, 65, 1234),
		Sample(SAMPLE_HEAT, 222, 37, 189, 65, 1234),
		Sample(SAMPLE_HEAT, 223, 37, 189, 66, 1234),
		Sample(SAMPLE_HEAT, 224, 37, 190, 66, 1234),
		Sample(SAMPLE_HEAT, 225, 37, 189, 66, 1234),
		Sample(SAMPLE_HEAT, 226, 37, 190, 67, 1234),
		Sample(SAMPLE_HEAT, 227, 37, 189, 67, 1234),
		Sample(SAMPLE_HEAT, 228, 37, 189, 67, 1234),
		Sample(SAMPLE_HEAT, 229, 37, 190, 68, 1234),
		Sample(SAMPLE_HEAT, 230, 38, 189, 68, 1234),
		Sample(SAMPLE_HEAT, 231, 38, 189, 68, 1234),
		Sample(SAMPLE_HEAT, 232, 38, 189, 69, 1234),
		Sample(SAMPLE_HEAT, 233, 38, 189, 69, 1234),
		Sample(SAMPLE_HEAT, 234, 38, 189, 69, 1234),
		Sample(SAMPLE_HEAT, 235, 38, 189, 70, 1234),
		Sample(SAMPLE_HEAT, 236, 38, 189, 70, 1234),
		Sample(SAMPLE_HEAT, 237, 38, 189, 70, 1234),
		Sample(SAMPLE_HEAT, 238, 38, 189, 71, 1234),
		Sample(SAMPLE_HEAT, 239, 38, 189, 71, 1234),
		Sample(SAMPLE_HEAT, 240, 38, 189, 71, 1234),
		Sample(SAMPLE_HEAT, 241, 39, 189, 72, 1234),
		Sample(SAMPLE_HEAT, 242, 39, 189, 72, 1234),
		Sample(SAMPLE_HEAT, 243, 39, 190, 72, 1234),
		Sample(SAMPLE_HEAT, 244, 39, 189, 73, 1234),
		Sample(SAMPLE_HEAT, 245, 39, 190, 73, 1234),
		Sample(SAMPLE_HEAT, 246, 39, 189, 73, 1234),
		Sample(SAMPLE_HEAT, 247, 39, 189, 74, 1234),
		Sample(SAMPLE_HEAT, 248, 39, 190, 74, 1234),
		Sample(SAMPLE_HEAT, 249, 39, 189, 75, 1234),
		Sample(SAMPLE_HEAT, 250, 39, 189, 75, 1234),
		Sample(SAMPLE_HEAT, 251, 39, 189, 75, 1234),
		Sample(SAMPLE_HEAT, 252, 39, 189, 76, 1234),
		Sample(SAMPLE_HEAT, 253, 39, 189, 76, 1234),
		Sample(SAMPLE_HEAT, 254, 40, 189, 76, 1234),
		Sample(SAMPLE_HEAT, 255, 40, 190, 76, 1234),
		Sample(SAMPLE_HEAT, 256, 40, 189, 77, 1234),
		Sample(SAMPLE_HEAT, 257, 40, 189, 77, 1234),
		Sample(SAMPLE_HEAT, 258, 40, 189, 77, 1234),
		Sample(SAMPLE_HEAT, 259, 40, 189, 78, 1234),
		Sample(SAMPLE_HEAT, 260, 40, 189, 78, 1234),
		Sample(SAMPLE_HEAT, 261, 40, 189, 78, 1234),
		Sample(SAMPLE_HEAT, 262, 40, 189, 78, 1234),
		Sample(SAMPLE_HEAT, 263, 40, 190, 79, 1234),
		Sample(SAMPLE_HEAT, 264, 40, 189, 79, 1234),
		Sample(SAMPLE_HEAT, 265, 40, 189, 79, 1234),
		Sample(SAMPLE_HEAT, 266, 40, 190, 80, 1234),
		Sample(SAMPLE_HEAT, 267, 40, 189, 80, 1234),
		Sample(SAMPLE_HEAT, 268, 41, 189, 80, 1234),
		Sample(SAMPLE_HEAT, 269, 41, 189, 81, 1234),
		Sample(SAMPLE_HEAT, 270, 41, 189, 81, 1234),
		Sample(SAMPLE_HEAT, 271, 41, 189, 81, 1234),
		Sample(SAMPLE_HEAT, 272, 41, 189, 81, 1234),
		Sample(SAMPLE_HEAT, 273, 41, 189, 82, 1234),
		Sample(SAMPLE_HEAT, 274, 41, 189, 82, 1234),
		Sample(SAMPLE_COOL, 275, 41, 180, 82, 1234),
		Sample(SAMPLE_COOL, 276, 41, 168, 82, 1234),
		Sample(SAMPLE_COOL, 277, 41, 157, 83, 1234),
		Sample(SAMPLE_COOL, 278, 41, 148, 83, 1234),
		Sample(SAMPLE_COOL, 279, 41, 139, 83, 1234),
		Sample(SAMPLE_COOL, 280, 41, 130, 83, 1234),
		Sample(SAMPLE_COOL, 281, 41, 123, 83, 1234),
		Sample(SAMPLE_COOL, 282, 41, 116, 84, 1234),
		Sample(SAMPLE_COOL, 283, 41, 111, 84, 1234),
		Sample(SAMPLE_COOL, 284, 41, 105, 84, 1234),
		Sample(SAMPLE_COOL, 285, 41, 100, 84, 1234),
		Sample(SAMPLE_COOL, 286, 41, 95, 84, 1234),
		Sample(SAMPLE_COOL, 287, 40, 91, 85, 1234),
		Sample(SAMPLE_COOL, 288, 40, 87, 85, 1234),
		Sample(SAMPLE_COOL, 289, 40, 83, 85, 1234),
		Sample(SAMPLE_COOL, 290, 40, 80, 85, 1234),
		Sample(SAMPLE_COOL, 291, 40, 76, 85, 1234),
		Sample(SAMPLE_COOL, 292, 40, 74, 85, 1234),
		Sample(SAMPLE_COOL, 293, 40, 71, 85, 1234),
		Sample(SAMPLE_COOL, 294, 40, 69, 85, 1234),
		Sample(SAMPLE_BREAKSEAL, 295, 39, 63, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 296, 39, 61, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 297, 39, 59, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 298, 39, 57, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 299, 39, 56, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 300, 39, 54, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 301, 39, 53, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 302, 38, 52, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 303, 38, 51, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 304, 38, 50, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 305, 38, 49, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 306, 38, 48, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 307, 38, 47, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 308, 37, 46, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 309, 37, 45, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 310, 37, 45, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 311, 37, 44, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 312, 37, 43, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 313, 37, 43, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 314, 36, 42, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 315, 36, 42, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 316, 36, 41, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 317, 36, 41, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 318, 36, 40, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 319, 36, 40, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 320, 36, 39, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 321, 35, 39, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 322, 35, 39, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 323, 35, 38, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 324, 35, 38, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 325, 35, 38, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 326, 35, 37, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 327, 34, 37, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 328, 34, 37, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 329, 34, 37, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 330, 34, 36, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 331, 34, 36, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 332, 34, 36, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 333, 34, 36, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 334, 34, 36, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 335, 34, 35, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 336, 33, 35, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 337, 33, 35, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 338, 33, 35, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 339, 33, 35, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 340, 33, 34, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 341, 33, 34, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 342, 33, 34, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 343, 33, 34, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 344, 32, 34, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 345, 32, 34, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 346, 32, 34, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 347, 32, 33, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 348, 32, 33, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 349, 32, 33, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 350, 32, 33, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 351, 32, 33, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 352, 32, 33, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 353, 32, 33, 2046, 1234),
		Sample(SAMPLE_BREAKSEAL, 354, 31, 33, 2046, 1234),
	};

#endif

	int LoadedSampleCount = 0;	
private:	
	Model* _model;
	LFT_Information * _information;
	Sample GetSample(int logIndex, int sampleIndex);

	ThreadSafe<int> quedLogIndex = 0;
	ThreadSafe<int> quedSampleCount = 0;
	ThreadSafe<bool> sampleLoadedAborted = false;
		

	static bool sortHeaders(RecoverLog a, RecoverLog b);
};


#endif /* MODEL_HPP */
