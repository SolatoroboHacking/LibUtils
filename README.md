# LibRipper

LibRipper is a basic extraction utility for the text of the library books in Solatorobo: Red the Hunter.
This utility extracts the text, and can convert it to either a readable layout, or a markdown layout suitable for use with LibCreate. It supports all six languages supported by the DS (Japanese, English, French, Spanish, German and Italian).

Starting from the game's root directory, the library files can be found in /data/etc/libparam*.ccb. There are different libparam files for every language the game supports.

These .ccb files are compressed archives. To extract them, you can use Console Tool or my own CCBExtractor utility, which can be found at https://github.com/SolatoroboHacking/CCBUtils

Inside those .ccb archives are two types of files: .cclbm and .cclhd. The contents and layouts of these formats is described below. They can both be extracted with this tool.

## Build Instructions

This project was designed to be built with MinGW for Cygwin for Windows binaries, or Linux using the g++ compiler. However, it will likely compile under any
C++ compiler.


  ### Cygwin instructions

  ```
  x86_64-w64-mingw32-g++ --static -o LibRipper.exe LibRipper.cpp
  ```

  ### Linux instructions

  ```
  g++ --static -o LibRipper LibRipper.cpp
  ```

## Usage Instructions

```
LibRipper [INPUT_FILE]
```

If the input file is a .cclbm (CyberConnect Library Material(?)) file, the output file will have the same name as the input file, but with the .cclbm extension removed and the .txt extension added. EX: libanimal_en.cclbm becomes libanimal_en.txt

If the input file is a .cclhd (CyberConnect Library Header) file, the output file will have the same name as the input file, but with the .txt extension added. EX: libanimal_en.cclhd becomes libanimal_en.cclhd.txt

Note: It is recommended to leave the filenames intact when using LibRipper, as it relies on the presence of the .cclhd file extension to determine if it is working with a header file or a library file.

# LibCreate

LibCreate is a utility that can be used to replace the text of any of the library books in Solatorobo for any language the game supports, as well as their titles and scrolling hint text.

LibCreate can generate .cclbm and .cclhd files that can be placed back into the original libparam*.ccb files using my own CCBConstructor utility (which can be found at https://github.com/SolatoroboHacking/CCBUtils) and then re-inserted into the DS ROM using a tool like Tinke. 

Text files should contain "<sec>" and "<end>" flags, 2 pairs of each for header files, and 6 pairs of each for library files (there are 3 sections, and each one has 2 pairs; one for title, one for content)

All content outside these sections will be ignored. However, the correct number of sections must be present

To see an example of this, extract one of the unmodified library files with LibRipper using the -m parameter.

LibCreate also sets how many repititions of a particular action the player must perform to unlock each section of the book. These parameters are required when generating a header file.

## Build Instructions

This project was designed to be built with MinGW for Cygwin for Windows binaries, or Linux using the g++ compiler. However, it will likely compile under any
C++ compiler.


  ### Cygwin instructions

  ```
  x86_64-w64-mingw32-g++ --static -o LibCreate.exe LibCreate.cpp
  ```

  ### Linux instructions

  ```
  g++ --static -o LibCreate LibCreate.cpp
  ```

## Usage Instructions

```
LibCreate [INPUT_FILE] [OUTPUT_FILE] (-h unlock1 unlock2 unlock3)
```

Use the -h parameter to generate a .cclhd header file, and supply 3 base-10 integers that will decide how many repitions the player must perform to unlock the corresponding book.

## Format Descriptions

This section is a brief format description of the file types used to store the library books in Solatorobo.

  ### CCLBM Files (CyberConnect Library Materials)

  These files contain the actual content of the library books, including section titles and text. They are limited to having three sections.
  The one thing they do not contain is the accompanying images that are displayed on the top screen. These graphics are located in /data/pic/pic3.ccb. There are 21 graphics in total, 3 for each section of the 7 library books.
  
  ```
  Header:
  4-byte "file type" value, usually decimal 3

  Sections:
  4-byte size value for the current section
  3-byte zero-width space section break 0xEFBBBF (Optional, not found in Japanese files)
  SECTION TITLE ENCODED AS UTF-8 followed by 0x00 terminator
  SECTION TEXT ENCODED AS UTF-8 (Line endings can be 0x0D0A or 0x0A (Windows or Unix) )
  One or more 0x00 null terminator bytes
  ```

  Some files may have unused data at the end that is left over from when the files were modified for each language. This data is unnecessary, but does not need to be removed either. So long as the rest of the file is          formatted properly, the game will simply ignore this data.

  ### CCLHD Files (CyberConnect Library Headers)
  These files contain the title of the library book that shows up in the Library menu, as well as the scrolling hint at the top of the Library menu that suggests how you unlock the book.
  ```
  Header:
  4-byte identifier (0x03000000)
  3 unsigned 32-bit integers - Each integer represents the number of times the player must
                               repeat an action to unlock a section of the book. The action
                               cannot be changed via this header, but the number of times it must be
                               repeated can.

  Sections:
  3-byte zero-width space section break 0xEFBBBF (Optional, not found in Japanese files)
  SECTION TEXT ENCODED AS UTF-8 (Line endings can be 0x0D0A or 0x0A (Windows or Unix) )
  One 0x00 null terminator byte

  Note: Header files have only 1 section with a title and its content.
  ```

  
  
## What is currently unknown?
There is currently no known method to change what action the player must repeat to unlock the books, though the number of repititions can be changed. It is also unknown if there is a way to bypass the three-section limit for each library book or change the total number of library books that exist. 
