
#ifdef __OBJC__

#include <GameLibrary/IO/FileTools.hpp>
#include <GameLibrary/Utilities/PlatformChecks.hpp>

#if defined(TARGETPLATFORM_MAC)
#import <AppKit/AppKit.h>
#endif

namespace fgl
{
#if defined(TARGETPLATFORM_MAC)
	
	String FileTools::openFilePicker(const String& title, const String& startingDir, bool fileExists, const String& defaultExtension)
	{
		if(fileExists)
		{
			NSOpenPanel* panel = [NSOpenPanel openPanel];
			[panel setCanChooseFiles:YES];
			[panel setCanChooseDirectories:NO];
			[panel setAllowsMultipleSelection:NO];
			if(startingDir.length() > 0)
			{
				[panel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:(const char*)startingDir] isDirectory:YES]];
			}
			if(defaultExtension.length() > 0)
			{
				panel.allowedFileTypes = @[[NSString stringWithUTF8String:(const char*)defaultExtension]];
				panel.allowsOtherFileTypes = YES;
			}
			[panel setTitle:[NSString stringWithUTF8String:(const char*)title]];
			NSInteger clicked = [panel runModal];
			if(clicked == NSFileHandlingPanelOKButton)
			{
				NSURL* url = [panel URL];
				return [[url path] UTF8String];
			}
		}
		else
		{
			NSSavePanel* panel = [NSSavePanel savePanel];
			if(startingDir.length() > 0)
			{
				[panel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:(const char*)startingDir] isDirectory:YES]];
			}
			if(defaultExtension.length() > 0)
			{
				panel.allowedFileTypes = @[[NSString stringWithUTF8String:(const char*)defaultExtension]];
				panel.allowsOtherFileTypes = YES;
			}
			[panel setTitle:[NSString stringWithUTF8String:(const char*)title]];
			NSInteger clicked = [panel runModal];
			if(clicked == NSFileHandlingPanelOKButton)
			{
				NSURL* url = [panel URL];
				return [[url path] UTF8String];
			}
		}
		return "";
	}
	
	String FileTools::openFolderPicker(const String& title, const String& startingDir)
	{
		NSOpenPanel *panel = [NSOpenPanel openPanel];
		[panel setCanChooseFiles:NO];
		[panel setCanChooseDirectories:YES];
		[panel setAllowsMultipleSelection:NO];
		if(startingDir.length() > 0)
		{
			[panel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:(const char*)startingDir] isDirectory:YES]];
		}
		[panel setTitle:[NSString stringWithUTF8String:(const char*)title]];
		NSInteger clicked = [panel runModal];
		if(clicked == NSFileHandlingPanelOKButton)
		{
			NSURL* url = [panel URL];
			return [[url path] UTF8String];
		}
		return "";
	}
	
#elif defined(TARGETPLATFORM_IOS)
	
	String FileTools::openFilePicker(const String& title, const String& startingDir, bool fileExists, const String& defaultExtension)
	{
		//TODO implement ios file picker
		return "";
	}
	
	String FileTools::openFolderPicker(const String& title, const String& startingDir)
	{
		//TODO implement ios folder picker
		return "";
	}
	
#endif
}

#endif
