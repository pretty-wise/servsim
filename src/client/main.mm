#include <stdio.h>
#include "renderer.h"
#include "../common/world.h"
#include "../common/vec3.h"
#import <Cocoa/Cocoa.h>

@interface ServsimView : NSOpenGLView
{
  NSTimer* animationTimer;
  Renderer* m_renderer;
  Game* m_game;
  Input m_input;
  vec3 m_cameraPosition;
}
@end

@implementation ServsimView

-(void)animationTimerFired:(NSTimer*)timer
{
  [self setNeedsDisplay:YES];
}

-(void)prepareOpenGL
{
  [super prepareOpenGL];

  m_cameraPosition = vec3(0.f, 0.f, 10.f);
	
#ifndef DEBUG
  GLint swapInterval = 1;
  [[self openGLContext] setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
  if (swapInterval == 0)
    NSLog(@"Error: Cannot set swap interval.");
#endif
  
  m_renderer = new Renderer();
  m_game = new Game();
}

-(void)keyDown:(NSEvent *)event
{
  const float cameraDelta = 0.1f;
  NSString* chars = [event charactersIgnoringModifiers];
  if([chars length] == 1) {
    unichar c = [chars characterAtIndex:0];
    if(c == NSLeftArrowFunctionKey) {
      m_cameraPosition.x -= cameraDelta;
    } else if(c == NSRightArrowFunctionKey) {
      m_cameraPosition.x += cameraDelta;
    } else if(c == NSUpArrowFunctionKey) {
      m_cameraPosition.y += cameraDelta;
    } else if(c == NSDownArrowFunctionKey) {
      m_cameraPosition.y -= cameraDelta;
    }
    
    if(c == 'a') {
      m_input.SetKeyDown(Input::Key::kLeft);
    }
    if(c == 'd') {
      m_input.SetKeyDown(Input::Key::kRight);
    }
    if(c == 'w') {
      m_input.SetKeyDown(Input::Key::kForward);
    }
    if(c == 's') {
      m_input.SetKeyDown(Input::Key::kBack);
    }
    if(c == 'r') {
    }
    if(c == 'f') {
    }
    return;
  }
  [super keyDown:event];
}

-(void)keyUp:(NSEvent *)event
{
  NSString* chars = [event charactersIgnoringModifiers];
  if([chars length] == 1) {
    unichar c = [chars characterAtIndex:0];
    if(c == 'a') {
      m_input.SetKeyUp(Input::Key::kLeft);
    }
    if(c == 'd') {
      m_input.SetKeyUp(Input::Key::kRight);
    }
    if(c == 'w') {
      m_input.SetKeyUp(Input::Key::kForward);
    }
    if(c == 's') {
      m_input.SetKeyUp(Input::Key::kBack);
    }
    if(c == 'r') {
    }
    if(c == 'f') {
    }
    return;
  }
  [super keyDown:event];
}

-(void)updateAndDrawDemoView:(int)width Height:(int)height
{
  if(m_input.IsAnyDown()) {
    m_game->UpdateInput(m_input);
  }
  m_game->Update(16.f);
  m_renderer->BeginScene(width, height);
  m_renderer->RenderWorld(m_cameraPosition, m_game->GetCurrentState());
  m_renderer->EndScene();

  [[self openGLContext] makeCurrentContext];
  [[self openGLContext] flushBuffer];

  if (!animationTimer)
    animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
}

-(void)reshape
{
  [[self openGLContext] update];
  [self updateAndDrawDemoView :800 Height:600];
}

-(void)drawRect:(NSRect)bounds
{
  [self updateAndDrawDemoView : bounds.size.width Height : bounds.size.height];
}

-(BOOL)acceptsFirstResponder
{
  return (YES);
}

-(BOOL)becomeFirstResponder
{
  return (YES);
}

-(BOOL)resignFirstResponder
{
  return (YES);
}

-(void)dealloc
{
  [super dealloc];
  animationTimer = nil;
  delete m_renderer;
}

@end


@interface ServsimAppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic, readonly) NSWindow* window;
@end

@implementation ServsimAppDelegate
@synthesize window = _window;

-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}

-(NSWindow*)window
{
  if (_window != nil)
    return (_window);
	
  float offset = 100.f;
  float width = 800.f;
  float height = 600.f;
  NSRect viewRect = NSMakeRect(offset, offset, offset + width, offset + height);

  _window = [[NSWindow alloc] initWithContentRect:viewRect styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskMiniaturizable|NSWindowStyleMaskResizable|NSWindowStyleMaskClosable backing:NSBackingStoreBuffered defer:YES];
  [_window setTitle:@"Servsim Window"];
  [_window setOpaque:YES];
  [_window makeKeyAndOrderFront:NSApp];
  [_window setAcceptsMouseMovedEvents:YES];

  return (_window);
}

-(void)setupMenu
{
	NSMenu* mainMenuBar = [[NSMenu alloc] init];
  NSMenu* appMenu;
  NSMenuItem* menuItem;

  appMenu = [[NSMenu alloc] initWithTitle:@"Servsim"];
  menuItem = [appMenu addItemWithTitle:@"Quit Servsim" action:@selector(terminate:) keyEquivalent:@"q"];
  [menuItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];

  menuItem = [[NSMenuItem alloc] init];
  [menuItem setSubmenu:appMenu];

  [mainMenuBar addItem:menuItem];

  appMenu = nil;
  [NSApp setMainMenu:mainMenuBar];
}

-(void)dealloc
{
  [super dealloc];
  _window = nil;
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// Make the application a foreground application (else it won't receive keyboard events)
	ProcessSerialNumber psn = {0, kCurrentProcess};
	TransformProcessType(&psn, kProcessTransformToForegroundApplication);

	// Menu
  [self setupMenu];

  NSOpenGLPixelFormatAttribute attrs[] =
  {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFADepthSize, 32,
    NSOpenGLPFAOpenGLProfile,
    NSOpenGLProfileVersion4_1Core
  };

  NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
  ServsimView* view = [[ServsimView alloc] initWithFrame:self.window.frame pixelFormat:format];
  format = nil;
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
  if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
    [view setWantsBestResolutionOpenGLSurface:YES];
#endif // MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
  [self.window setContentView:view];

  if ([view openGLContext] == nil)
    NSLog(@"No OpenGL Context!");
}

@end



int main(int argc, const char* argv[])
{
	@autoreleasepool
	{
		NSApp = [NSApplication sharedApplication];
		ServsimAppDelegate* delegate = [[ServsimAppDelegate alloc] init];
		[[NSApplication sharedApplication] setDelegate:delegate];
		[NSApp run];
	}
	return NSApplicationMain(argc, argv);
}
