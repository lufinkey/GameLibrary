
#include <GameLibrary/Window/MessageBox.hpp>
#include <GameLibrary/Utilities/PlatformChecks.hpp>
#include <GameLibrary/Utilities/Thread.hpp>
#include "../Application/EventManager.hpp"
#include <SDL.h>



#if defined(TARGETPLATFORM_IOS)

#include "../PlatformSpecific/iOS/SystemVersionMacros.h"
#include "../PlatformSpecific/iOS/iOSUtils.h"


@interface FGLMessageBoxHandler : NSObject <UIAlertViewDelegate>
@property (nonatomic) NSInteger result;
@end

@implementation FGLMessageBoxHandler
@synthesize result = _result;
-(id)init
{
	if(self = [super init])
	{
		_result = (NSInteger)-1;
	}
	return self;
}
- (void)alertView:(UIAlertView*)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
	_result = buttonIndex;
}
- (void)alertView:(UIAlertView*)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	_result = buttonIndex;
}
@end


namespace fgl
{
	unsigned int MessageBox::show(Window* parent, const String& title, const String& message)
	{
		@autoreleasepool
		{
			NSString* titleStr = nil;
			if(title.length() > 0)
			{
				titleStr = (NSString*)title;
			}
			NSString* messageStr = nil;
			if(message.length() > 0)
			{
				messageStr = (NSString*)message;
			}
			
			if(SYSTEM_VERSION_LESS_THAN(@"8.0"))
			{
				UIAlertView *alert = [[UIAlertView alloc] initWithTitle:titleStr
																message:messageStr
																delegate:nil
																cancelButtonTitle:@"OK"
																otherButtonTitles:nil];
				[alert show];
				EventManager::update(false);
				while(alert.visible)
				{
					Thread::sleep(16);
					EventManager::update(false);
				}
				return 0;
			}
			else
			{
				UIViewController* rootVC = Window_getRootViewController(parent);
				if(rootVC==nil)
				{
					return -1;
				}
				
				UIAlertController* alert = [UIAlertController alertControllerWithTitle:titleStr message:messageStr preferredStyle:UIAlertControllerStyleAlert];
				[alert addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];
				[rootVC presentViewController:alert animated:YES completion:nil];
				
				EventManager::update(false);
				while(alert.presentingViewController != nil)
				{
					Thread::sleep(16);
					EventManager::update(false);
				}
				return 0;
			}
		}
	}
	
	unsigned int MessageBox::show(Window* parent, const String& title, const String& message, const ArrayList<String>& options)
	{
		@autoreleasepool
		{
			NSString* titleStr = nil;
			if(title.length() > 0)
			{
				titleStr = (NSString*)title;
			}
			NSString* messageStr = nil;
			if(message.length() > 0)
			{
				messageStr = (NSString*)message;
			}
			
			if(SYSTEM_VERSION_LESS_THAN(@"8.0"))
			{
				FGLMessageBoxHandler* messageHandler = [[FGLMessageBoxHandler alloc] init];
				UIAlertView *alert = [[UIAlertView alloc] initWithTitle:titleStr
																message:messageStr
															   delegate:messageHandler
													  cancelButtonTitle:nil
													  otherButtonTitles:nil];
				
				for(size_t i=0; i<options.size(); i++)
				{
					[alert addButtonWithTitle:(NSString*)options[i]];
				}
				[alert show];
				
				EventManager::update(false);
				while(alert.visible)
				{
					Thread::sleep(16);
					EventManager::update(false);
				}
				NSInteger result = messageHandler.result;
				return (unsigned int)result;
			}
			else
			{
				UIViewController* rootVC = Window_getRootViewController(parent);
				if(rootVC==nil)
				{
					return -1;
				}
				
				UIAlertController* alert = [UIAlertController alertControllerWithTitle:(NSString*)title message:(NSString*)message preferredStyle:UIAlertControllerStyleAlert];
				
				__block unsigned int selectedIndex = -1;
				for(size_t i=0; i<options.size(); i++)
				{
					[alert addAction:[UIAlertAction actionWithTitle:(NSString*)options[i] style:UIAlertActionStyleDefault handler:^(UIAlertAction* action){
						selectedIndex = (unsigned int)i;
					}]];
				}
				
				[rootVC presentViewController:alert animated:YES completion:nil];
				
				EventManager::update(false);
				while(alert.presentingViewController != nil)
				{
					Thread::sleep(16);
					EventManager::update(false);
				}
				
				return selectedIndex;
			}
		}
	}
}

#endif
