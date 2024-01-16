# LibRipper

LibRipper is a basic extraction utility for the text of the library books in Solatorobo: Red the Hunter.
At present, this utility only extracts the text, and puts it in a basic layout. However, it supports all six languages supported by the DS (Japanese, English, French, Spanish, German and Italian).

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

## Format Descriptions

This section is a brief format descriiption of the file types used to store the library books in Solatorobo.

  ### CCLBM Files (CyberConnect Library Materials)

  These files contain the actual content of the library books, including titles and text. They are limited to having three sections.
  The one thing they do not contain is the accompanying images that are displayed on the top screen.
  
  ```
  Header:
  4-byte "file type" value, usually decimal 3

  Sections:
  4-byte size value for the current section
  3-byte zero-width space section break 0xEFBBBF (Optional, not found in Japanese files)
  SECTION TEXT ENCODED AS UTF-8 (Line endings can be 0x0D0A or 0x0A (Windows or Unix) )
  One or more 0x00 null terminator bytes
  ```

  Some files may have unused data at the end that is left over from when the files were modified for each language. This data is unnecessary, but does not need to be removed either. So long as the rest of the file is formatted properly, the game will simply ignore this data.

  ### CCLHD Files (CyberConnect Library Headers)
  ```
  Header:
  16-byte header, values are currently not understood, but seem to be ignored by the game as long as first byte is 0x03
  3-byte zero-width space section break 0xEFBBBF (Optional, not found in Japanese files)
  SECTION TEXT ENCODED AS UTF-8 (Line endings can be 0x0D0A or 0x0A (Windows or Unix) )
  One 0x00 null terminator byte
  ```

  
  