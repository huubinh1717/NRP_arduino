#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include "utils/dbg.h"

// Helper function to convert a date-time string to std::tm
std::tm stringToTime(const std::string& timeStr) {
	std::tm tm = {};
	std::istringstream ss(timeStr);
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
	return tm;
}

// Helper function to compare two times
bool isTimeInRange(const std::tm& time, const std::tm& start, const std::tm& end) {
	std::time_t time_t_time = std::mktime(const_cast<std::tm*>(&time));
	std::time_t time_t_start = std::mktime(const_cast<std::tm*>(&start));
	std::time_t time_t_end = std::mktime(const_cast<std::tm*>(&end));

	return (time_t_time >= time_t_start && time_t_time <= time_t_end);
}

struct DataRow {
	std::string Time;
	std::vector<std::string> other_entries;  // Holds other 9 entries
	std::string Label;
};

struct TimeRange {
	std::tm start_time;
	std::tm end_time;
	std::string label;
};

int main() {
	// File paths for input CSVs
	std::string file1 = "THS.csv";  // First file (with 'Time' and 9 other columns)
	std::string file2 = "watch-sleep-data.csv";  // Second file (with 'start_time', 'end_time', and 'LABEL')

	// Read the first CSV file (df1) into a vector of DataRow
	std::vector<DataRow> df1;
	std::ifstream infile1(file1);
	std::string line;
	std::getline(infile1, line);  // Skip the header line
	while (std::getline(infile1, line)) {
		std::istringstream ss(line);
		std::string timeStr;
		std::getline(ss, timeStr, ',');  // Get 'Time' column

		DataRow row;
		row.Time = timeStr;

		// Get the next 9 entries (other columns)
		for (int i = 0; i < 3; ++i) {
			std::string entry;
			std::getline(ss, entry, ',');
			row.other_entries.push_back(entry);
		}

		df1.push_back(row);
	}
	infile1.close();

	// Read the second CSV file (df2) into a vector of TimeRange
	std::vector<TimeRange> df2;
	std::ifstream infile2(file2);
	std::getline(infile2, line);  // Skip the header line

	while (std::getline(infile2, line)) {
		std::istringstream ss(line);
		std::string start_time_str, end_time_str, label_str;

		// Skip the first 4 unused columns
		for (int i = 0; i < 4; ++i) {
			std::string unused;
			std::getline(ss, unused, ',');
		}

		// Read start_time, end_time, and label from the CSV
		std::getline(ss, start_time_str, ',');
		std::getline(ss, end_time_str, ',');
		std::getline(ss, label_str, ',');
		// Skip the final unused column
		std::string unused_end_col;
		std::getline(ss, unused_end_col, ',');

		std::cout << start_time_str << '\t' << end_time_str << std::endl;
		TimeRange range;
		range.start_time = stringToTime(start_time_str);
		range.end_time = stringToTime(end_time_str);
		range.label = label_str;
		df2.push_back(range);
	}
	infile2.close();

	// ! dsfsd

	// Now compare each Time in df1 with all time ranges in df2
	for (auto& row : df1) {
		std::tm timeToMatch = stringToTime(row.Time);
		bool matched = false;

		// Compare with each time range in df2
		for (const auto& range : df2) {
			if (isTimeInRange(timeToMatch, range.start_time, range.end_time)) {
				row.Label = range.label;
				matched = true;
				break;  // Once we find a match, no need to check further ranges
			}
		}

		// If no match is found, we can optionally set a default label
		if (!matched) {
			row.Label = "No Match";
		}
	}

	// Write the updated data to a new CSV file
	std::ofstream outfile("THS_with_labels.csv");
	outfile << "Time";
	for (int i = 0; i < 3; ++i) {
		outfile << ",Column" << (i + 1);
	}
	outfile << ",Label\n";

	for (const auto& row : df1) {
		outfile << row.Time;
		for (const auto& entry : row.other_entries) {
			outfile << "," << entry;
		}
		outfile << "," << row.Label << "\n";
	}

	outfile.close();

	std::cout << "CSV processing completed successfully. Output saved to 'df1_with_labels.csv'.\n";
	return 0;
}
