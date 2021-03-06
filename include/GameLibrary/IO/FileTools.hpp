
#pragma once

#include <cstdio>
#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Utilities/ArrayList.hpp>

namespace fgl
{
	/*! Handles reading directories and file structures from the disk*/
	class FileTools
	{
	public:
		/*! A type of directory item in a directory*/
		typedef enum
		{
			/*! A regular file*/
			ENTRYTYPE_FILE,
			/*! A regular folder*/
			ENTRYTYPE_FOLDER,
			/*! A symbolic link to a file*/
			ENTRYTYPE_LINK_FILE,
			/*! A symbolic link to a folder*/
			ENTRYTYPE_LINK_FOLDER,
			/*! A symbolic link whose type is unknown*/
			ENTRYTYPE_LINK,
			/*! An unknown directory entry*/
			ENTRYTYPE_UNKNOWN
		} DirectoryEntryType;
		
		
		/*! Holds information about an entry in a directory*/
		typedef struct
		{
			/*! The name of the directory entry*/
			String name;
			/*! The type of directory entry*/
			DirectoryEntryType type;
		} DirectoryEntry;
		
		
		/*! Gets the user home directory
			\returns a String containing the path to the current user's home directory, or an empty String if an error occured*/
		static String getUserHomeDirectory();
		
		
		/*! Gets the path for the current working directory.
			\returns a String containing the path to the working directory*/
		static String getCurrentWorkingDirectory();
		/*! Sets the path for the current working directory.
			\param directory a path to the new working directory
			\returns true if the working directory was successfully set, or false if there was an error */
		static bool setCurrentWorkingDirectory(const String& directory);
		/*! Stores a list of items in a directory inside of an ArrayList of DirectoryEntry objects.
			\param directory the directory to read entries
			\returns a list of entries
		 	\throws fgl::Exception if an error occurs */
		static ArrayList<DirectoryEntry> readEntriesFromDirectory(const String& directory);
		
		
		/*! Checks whether there are files or folder inside the specified directory. This function assumes the directory exists and is accessible.
			\param directory the directory to read
			\returns true if the directory exists and is empty, or false if it contains files or folder or an error occurred*/
		static bool isDirectoryEmpty(const String& directory);
		
		
		/*! Checks if a given path is an absolute path or a relative path
			\param path the path to check
			\returns true if the path is absolute, or false if the path is relative or is not a valid path */
		static bool isPathAbsolute(const String& path);
		/*! Expands a relative path to an absolute path
			\param path a path to expand to an absolute path
			\returns an absolute path, or an empty string if the path could not be expanded */
		static String getAbsolutePath(const String& path);
		/*! Gets a path relative to a given directory, from a given path
			\param directory the directory that the output path will be relative to
			\param path the path to compress to a relative path
			\returns a relative path, or an empty string if a relative path could not be made */
		static String getRelativePath(const String& directory, const String& path);
		/*! Combines two path strings into a single path.
			\param first the first half of the path
			\param second the second half of the path
			\returns a String containing a full path from the two given paths*/
		static String combinePathStrings(const String& first, const String& second);


		/*! Breaks a path into the directory and filename components and returns the directory component
			\param path the path to retrieve the directory name for
			\returns the directory component of the path */
		static String getDirectoryComponent(const String& path);
		
		
		/*! Opens a FILE pointer from a given file path
			\param path the path to the file
			\param mode C string containing the file access mode. \see std::fopen
			\param error a pointer to store an error string, if an error occurs
			\returns a valid FILE pointer if the file was successfully loaded, or false if an error occurred */
		static FILE* openFile(const String& path, const char* mode, String* error=nullptr);
		/*! Closes an open FILE pointer
			\param file the open FILE pointer */
		static void closeFile(FILE* file);
		
		
		/*! Opens up the native file chooser to select a file.
			\param title the title text on the file picker window
			\param startingDir the path to open the file picker window to; default value is the user home directory
			\param fileExists specifies whether the file must exist or not
			\param defaultExtension the default file extension to show/use
			\returns a String representing the selected path, or an empty String if the operation was cancelled*/
		static String openFilePicker(const String& title="Open", const String& startingDir="", bool fileExists=true, const String& defaultExtension="");
		
		
		/*! Opens up the native file chooser to select a folder.
			\param title the title text on the folder picker window
			\param startingDir the path to open the file picker window to; default value is the user home directory
			\returns a String representing the selected path, or an empty String if the operation was cancelled*/
		static String openFolderPicker(const String& title="Open", const String& startingDir="");
	};
}
