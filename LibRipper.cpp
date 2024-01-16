/*/////////////////////////////////////////////////////////////////

LibRipper

Copyright 2023 (C) - SolatoroboHacking

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

using namespace std;

// Function to convert the raw text from the library files into a more readable layout.
// This essentially just adds lines of ------- to break up sections
void convertReadable(unsigned char* inputText, FILE* outputFile, uint32_t sectionSize) {
	// For every byte in the current section,
	for (uint32_t i = 0; i < sectionSize; i++) {
		// If there is a null byte,
		if (inputText[i] == 0x00) {
			// Skip over all the null bytes
			while (inputText[i+1] == 0x00 && i < sectionSize - 1) {
				i++;
			}
			// If we are not at the end of the section,
			if (i < sectionSize - 1) {
				// Add a line under the title
				fputs("\n---------------------\n\n", outputFile);
			}
			// If the file contains 0xEFBBBF section breaks,
		} else if (inputText[i] == 0xEF && inputText[i+1] == 0xBB && inputText[i+2] == 0xBF) {
			// Skip over them
			i = i + 2;
			// The only real terminator that the game recognizes is 0x00, a null byte, to terminate the current section/string.
			// 0xEFBBBF are not printed, as they are zero-width spaces in Unicode.
			// They are not written to the output.

			// If the current character is just a regular character,
		} else {
			// Write it to the output
			fputc(inputText[i], outputFile);
		}
	}
	// Return when done with the current section
	return;
}


// Main function
int main(int argc, char** argv) {
	// If no input file was provided,
	if (argc < 2) {
		// Error out
		cout << "Error: Provide input file" << endl;
		return 1;
	}
	// Open source file
	FILE* inputFile = fopen(argv[1], "rb");
	// Define a pointer for output file to be written
	FILE* outputFile;
	// If the input file is a .cclhd file, or a "CyberConnect Library Header" file,
	if (string(argv[1]).find(".cclhd") != -1) {
		// Open an output file called "[SOURCE_NAME].cclhd.txt"
		FILE* outputFile = fopen(string(argv[1]).append(".txt").c_str(), "wb");
		// Go to offset 0x10, decimal 16, as this is where the content starts.
		// The header data for these files, despite being 16 bytes long, is not very useful
		fseek(inputFile, 16, SEEK_SET);
		// Run the following code twice; once for the section title, once for the hint
		for (int i = 0; i < 2; i++) {
			// Define an unsigned char to hold current byte (we do not know or need to know the size of the section)
			unsigned char currentByte = fgetc(inputFile);
			// While the current byte is not 0x00, the null terminator,
			while (currentByte != 0x00) {
				// Check if there is a 0xEFBBBF separator. If there is not, backtrack and continue
				if (currentByte == 0xEF) {
					currentByte = fgetc(inputFile);
					if (currentByte == 0xBB) {
						currentByte = fgetc(inputFile);
						if (currentByte == 0xBF) {
							currentByte = fgetc(inputFile);
						} else {
							fseek(inputFile, -3, SEEK_CUR);
							currentByte = fgetc(inputFile);
						}
					} else {
						fseek(inputFile, -2, SEEK_CUR);
						currentByte = fgetc(inputFile);
					}
				}
				// Write the current byte to the output file
				fputc(currentByte, outputFile);
				// Read the next input byte
				currentByte = fgetc(inputFile);
			}
			// Write line breaks after each section
			fputs("\n\n", outputFile);
		}
		// Close the output file when done
		fclose(outputFile);
	} else {
		// Define a new array pointer to hold buffer
		unsigned char* readBytes = new unsigned char[4];
		// Open output file to be written
		outputFile = fopen(string(argv[1]).substr(0, string(argv[1]).find(".cclbm")).append(".txt").c_str(), "wb");
		// Skip over the first 4-bytes of the file (file type identifier, not useful)
		fseek(inputFile, 4, SEEK_SET);
		// For each of the three sections (title + content) in the file,
		for (int i = 0; i < 3; i++) {
			// Read 4-byte size value of current section from source file
			fread(readBytes, 1, 4, inputFile);
			// Convert these 4 bytes into an unsigned 32-bit integer
			uint32_t sectionSize = ((uint32_t)(readBytes[3] << 24) + (uint32_t)(readBytes[2] << 16) + (uint32_t)(readBytes[1] << 8) + (uint32_t)readBytes[0]);
			// This size value, for some reason, includes the 4-bytes of the size value, so subtract them.
			sectionSize = sectionSize - 4;
			// Read the current section into the buffer
			readBytes = new unsigned char[sectionSize];
			fread(readBytes, 1, sectionSize, inputFile);
			// Send the current section off to be converted into a readable layout
			convertReadable(readBytes, outputFile, sectionSize);
			// If we are not at the last section, write a section break when done with the current section
			if (i != 2) {
				fputs("\n---------------------\n\n\n", outputFile);
			}
		}
		// Close the output file
		fclose(outputFile);
	}
	// Close the input file
	fclose(inputFile);
	return 0;
}