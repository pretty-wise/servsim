#include <stdio.h>
#include "renderer.h"
#include "../common/vec3.h"
#import <Cocoa/Cocoa.h>

@interface ServsimView : NSOpenGLView
{
  NSTimer* animationTimer;
  Renderer* m_renderer;
  
  vec3 m_modelTranslation;
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
  
  m_modelTranslation = vec3(0.f, 0.f, 0.f);
  m_cameraPosition = vec3(0.f, 0.f, 10.f);
	
#ifndef DEBUG
  GLint swapInterval = 1;
  [[self openGLContext] setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
  if (swapInterval == 0)
    NSLog(@"Error: Cannot set swap interval.");
#endif
  
  m_renderer = new Renderer();
}

-(void)keyDown:(NSEvent *)event
{
  const float translationDelta = 0.1f;
  NSString* chars = [event charactersIgnoringModifiers];
  if([chars length] == 1) {
    unichar c = [chars characterAtIndex:0];
    if(c == NSLeftArrowFunctionKey) {
      m_modelTranslation.x -= translationDelta;
    } else if(c == NSRightArrowFunctionKey) {
      m_modelTranslation.x += translationDelta;
    } else if(c == NSUpArrowFunctionKey) {
      m_modelTranslation.y += translationDelta;
    } else if(c == NSDownArrowFunctionKey) {
      m_modelTranslation.y -= translationDelta;
    }
    
    const float cameraDelta = 0.1f;
    if(c == 'a') {
      m_cameraPosition.x -= cameraDelta;
    } else if(c == 'd') {
      m_cameraPosition.x += cameraDelta;
    } else if(c == 'w') {
      m_cameraPosition.y += cameraDelta;
    } else if(c == 's') {
      m_cameraPosition.y -= cameraDelta;
    } else if(c == 'r') {
      m_cameraPosition.z -= cameraDelta;
    } else if(c == 'f') {
      m_cameraPosition.z += cameraDelta;
    }
    return;
  }
  [super keyDown:event];
}

-(void)keyUp:(NSEvent *)event
{
  
}

-(void)updateAndDrawDemoView:(int)width Height:(int)height
{
  m_renderer->BeginScene(width, height);
  m_renderer->RenderScene(m_cameraPosition, m_modelTranslation);
  //m_renderer->RenderScene(m_cameraPosition, m_modelTranslation + vec3(2.5f, 0.f, 0.f));
  m_renderer->EndScene();

// Rendering
//ImGui::Render();
[[self openGLContext] makeCurrentContext];

  //ImGuiIO& io = ImGui::GetIO();
  //GLsizei width  = (GLsizei)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
  //GLsizei height = (GLsizei)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
  //glViewport(0, 0, width, height);

  //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  //glClear(GL_COLOR_BUFFER_BIT);
  //ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  // Present
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
