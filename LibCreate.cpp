/*/////////////////////////////////////////////////////////////////

LibRipper

Copyright 2024 (C) - SolatoroboHacking

This program is free software: you can redistribute it and/or 
modify it under the terms of the GNU General Public License as 
published by the Free Software Foundation, either version 3 of the 
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program. 
If not, see <https://www.gnu.org/licenses/>. 

*//////////////////////////////////////////////////////////////////

// Importing essential libraries
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>

using namespace std;

// Main function
int main(int argc, char** argv) {
	// If no input file was provided,
	if (argc < 2) {
		// Error out
		cout << "Error: Provide input file" << endl;
		return 1;
		// If no output file was provided,
	} else if (argc < 3) {
		// Error out
		cout << "Error: Provide output file" << endl;
	} else {
		// If the input file does not exist,
		if (!filesystem::exists(argv[1])) {
			// Error out
			cout << "Error: Input file does not exist" << endl;
			return 1;
		} else {
			bool GENERATE_HEADER;
			// If the -h paramater was passed, set a flag to generate a .cclhd header file
			if (argc >= 4 && strcmp(argv[3], "-h") == 0) {
				GENERATE_HEADER = true;
			}
			// Open source file
			FILE* inputFile = fopen(argv[1], "rb");
			// Define a pointer for the output file to be written
			FILE* outputFile = fopen(argv[2], "wb");
			// Write 4-byte identifier file
			fputc(0x03, outputFile);
			fputc(0x00, outputFile);
			fputc(0x00, outputFile);
			fputc(0x00, outputFile);
			// Normal .cclbm files have three sections. This is the default.
			int sectionCount = 3;
			// If generating a header,
			if (GENERATE_HEADER) {
				// Check if unlock requirements were provided
				if (argc < 7) {
					// If not, error out
					cout << "Error: headers require 3 section unlock requirement values" << endl;
					return 1;
					// Otherwise,
				} else {
					// If generating a header, there is only one section
					sectionCount = 1;
					// For each of the three sections, write their unlock requirements
					for (int i = 4; i < 7; i++) {
						uint32_t unlock = (uint32_t)stoul(argv[i]);
						fputc((unsigned char)unlock, outputFile);
						fputc((unsigned char)(unlock >> 8), outputFile);
						fputc((unsigned char)(unlock >> 16), outputFile);
						fputc((unsigned char)(unlock >> 24), outputFile);
					}
				}
			}
			// For every section to be written,
			for (int i = 0; i < sectionCount; i++) {
				// Keep track of where in the output file the current section starts
				uint32_t sectionStart = ftell(outputFile);
				// If NOT generating a header, write 4 bytes that will be written with the size of the section
				if (!GENERATE_HEADER) {
					fputc(0x00, outputFile);
					fputc(0x00, outputFile);
					fputc(0x00, outputFile);
					fputc(0x00, outputFile);
				}
				// Run the following code twice, once for the title and once for the content
				for (int j = 0; j < 2; j++) {
					unsigned char currentByte = fgetc(inputFile);
					// While we have not reached the end of the input file,
					while (currentByte != EOF) {
						// Check for a section start flag. If one is not found, backtrack and continue
						if (currentByte == '<') {
							currentByte = fgetc(inputFile);
							if (currentByte == 's') {
								currentByte = fgetc(inputFile);
								if (currentByte == 'e') {
									currentByte = fgetc(inputFile);
									if (currentByte == 'c') {
										currentByte = fgetc(inputFile);
										if (currentByte == '>') {
											break;
										} else {
											fseek(inputFile, -4, SEEK_CUR);
										}
									} else {
										fseek(inputFile, -3, SEEK_CUR);
									}
								} else {
									fseek(inputFile, -2, SEEK_CUR);
								}
							} else {
								fseek(inputFile, -1, SEEK_CUR);
							}
						}
						// Read the next input byte
						currentByte = fgetc(inputFile);
					}
					// If the end of the file has been reached, the file must be invalid, so error out
					if (currentByte == EOF) {
						cout << "Error: End of file reached prematurely" << endl;
						fclose(inputFile);
						fclose(outputFile);
						return 1;
					}
					currentByte = fgetc(inputFile);
					// While we have not reached the end of the input file,
					while (currentByte != EOF) {
						// Check for a section end flag. If one is not found, backtrack and continue
						if (currentByte == '<') {
							currentByte = fgetc(inputFile);
							if (currentByte == 'e') {
								currentByte = fgetc(inputFile);
								if (currentByte == 'n') {
									currentByte = fgetc(inputFile);
									if (currentByte == 'd') {
										currentByte = fgetc(inputFile);
										if (currentByte == '>') {
											break;
										} else {
											fseek(inputFile, -5, SEEK_CUR);
											currentByte = fgetc(inputFile);
										}
									} else {
										fseek(inputFile, -4, SEEK_CUR);
										currentByte = fgetc(inputFile);
									}
								} else {
									fseek(inputFile, -3, SEEK_CUR);
									currentByte = fgetc(inputFile);
								}
							} else {
								fseek(inputFile, -2, SEEK_CUR);
								currentByte = fgetc(inputFile);
							}
						}
						fputc(currentByte, outputFile);
						currentByte = fgetc(inputFile);
					}
					// If the end of the file has been reached, the file must be invalid, so error out.
					if (currentByte == EOF) {
						cout << "Error: End of file reached prematurely" << endl;
						fclose(inputFile);
						fclose(outputFile);
						return 1;
					}
					// Write a null terminator to end the section.
					fputc(0x00, outputFile);
				}
				// If generating a .cclbm file, NOT a header,
				if (!GENERATE_HEADER) {
					// Get current position of the output file pointer
					uint32_t sectionEnd = ftell(outputFile);
					// Calculate the size of the section (the size includes the 4 bytes of the size value),
					uint32_t sectionSize = sectionEnd - sectionStart;
					fseek(outputFile, sectionStart, SEEK_SET);
					// And write it to the 4 padding bytes written earlier
					fputc((unsigned char)sectionSize, outputFile);
					fputc((unsigned char)(sectionSize >> 8), outputFile);
					fputc((unsigned char)(sectionSize >> 16), outputFile);
					fputc((unsigned char)(sectionSize >> 24), outputFile);
					fseek(outputFile, sectionEnd, SEEK_SET);
				}
			}
			// Close the input and output files when done
			fclose(inputFile);
			fclose(outputFile);
		}
	}
	return 0;
}