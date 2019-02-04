#include <stdio.h>
#include <math.h>
#import <Cocoa/Cocoa.h>
#include "common/world.h"
#include "common/vec3.h"
#include "common/vec2.h"
#include "renderer.h"

struct DeviceInput
{
  enum class ButtonState { kNone, kPressed, kDown, kReleased };
  vec2 m_mouse;
  vec2 m_mouse_normalized;
  ButtonState m_mouse_left;
  ButtonState m_mouse_right;
  ButtonState m_button_w;
  ButtonState m_button_s;
  ButtonState m_button_a;
  ButtonState m_button_d;
  ButtonState m_arrow_up;
  ButtonState m_arrow_down;
  ButtonState m_arrow_left;
  ButtonState m_arrow_right;
  
  bool IsDown(ButtonState state) const {
    return state == ButtonState::kPressed
    || state == ButtonState::kDown;
  }
  
  void PostUpdate() {
    PostUpdate(m_button_w);
    PostUpdate(m_button_s);
    PostUpdate(m_button_a);
    PostUpdate(m_button_d);
    PostUpdate(m_arrow_up);
    PostUpdate(m_arrow_down);
    PostUpdate(m_arrow_left);
    PostUpdate(m_arrow_right);
    PostUpdate(m_mouse_left);
    PostUpdate(m_mouse_right);
  }
  
private:
  static void PostUpdate(ButtonState& s) {
    if(s == ButtonState::kPressed) s = ButtonState::kDown;
    if(s == ButtonState::kReleased) s = ButtonState::kNone;
  }
};

Input create_input(const DeviceInput& device) {
  Input input;
  if(device.IsDown(device.m_button_w)) input.SetKeyDown(Input::Key::kForward);
  if(device.IsDown(device.m_button_s)) input.SetKeyDown(Input::Key::kBack);
  if(device.IsDown(device.m_button_a)) input.SetKeyDown(Input::Key::kLeft);
  if(device.IsDown(device.m_button_d)) input.SetKeyDown(Input::Key::kRight);
  return input;
}

@interface ServsimView : NSOpenGLView
{
  NSTimer* animationTimer;
  Renderer* m_renderer;
  Game* m_game;
  DeviceInput m_input;
  vec3 m_camera_position;
  vec2 m_window_dimension;
  
  vec2 m_mouse_drag_start;
  vec3 m_camera_forward;
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

  m_camera_position = vec3(-4.f, 6.f, 10.f);
  m_camera_forward = vec3::normalize(-m_camera_position);//-vec3::kUnitZ;
	
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
  NSString* chars = [event charactersIgnoringModifiers];
  if([chars length] == 1) {
    unichar c = [chars characterAtIndex:0];
    if(c == NSLeftArrowFunctionKey) m_input.m_arrow_left = DeviceInput::ButtonState::kPressed;
    if(c == NSRightArrowFunctionKey) m_input.m_arrow_right = DeviceInput::ButtonState::kPressed;
    if(c == NSUpArrowFunctionKey) m_input.m_arrow_up = DeviceInput::ButtonState::kPressed;
    if(c == NSDownArrowFunctionKey) m_input.m_arrow_down = DeviceInput::ButtonState::kPressed;
    
    if(c == 'a') m_input.m_button_a = DeviceInput::ButtonState::kPressed;
    if(c == 'd') m_input.m_button_d = DeviceInput::ButtonState::kPressed;
    if(c == 'w') m_input.m_button_w = DeviceInput::ButtonState::kPressed;
    if(c == 's') m_input.m_button_s = DeviceInput::ButtonState::kPressed;
    if(c == 'r') {}
    if(c == 'f') {}
    return;
  }
  [super keyDown:event];
}

-(void)keyUp:(NSEvent *)event
{
  NSString* chars = [event charactersIgnoringModifiers];
  if([chars length] == 1) {
    unichar c = [chars characterAtIndex:0];
    if(c == NSLeftArrowFunctionKey) m_input.m_arrow_left = DeviceInput::ButtonState::kReleased;
    if(c == NSRightArrowFunctionKey) m_input.m_arrow_right = DeviceInput::ButtonState::kReleased;
    if(c == NSUpArrowFunctionKey) m_input.m_arrow_up = DeviceInput::ButtonState::kReleased;
    if(c == NSDownArrowFunctionKey) m_input.m_arrow_down = DeviceInput::ButtonState::kReleased;
    if(c == 'a') m_input.m_button_a = DeviceInput::ButtonState::kReleased;
    if(c == 'd') m_input.m_button_d = DeviceInput::ButtonState::kReleased;
    if(c == 'w') m_input.m_button_w = DeviceInput::ButtonState::kReleased;
    if(c == 's') m_input.m_button_s = DeviceInput::ButtonState::kReleased;
    if(c == 'r') {}
    if(c == 'f') {}
    return;
  }
  [super keyDown:event];
}

-(void)mouseDown:(NSEvent *)event
{
  m_input.m_mouse_left = DeviceInput::ButtonState::kPressed;
  NSPoint mousePoint = [event locationInWindow];
  printf("mouse down: %f %f\n", mousePoint.x, mousePoint.y);
}

-(void)mouseUp:(NSEvent *)event
{
  m_input.m_mouse_left = DeviceInput::ButtonState::kReleased;
  NSPoint mousePoint = [event locationInWindow];
  printf("mouse up  : %f %f\n", mousePoint.x, mousePoint.y);
}

static float clamp(float v, float min, float max) {
  if(v < min) return min;
  if(v > max) return max;
  return v;
}
-(void)updateMouse:(NSEvent *)event
{
  NSPoint mousePoint = [event locationInWindow];
  m_input.m_mouse = vec2(clamp(mousePoint.x, 0, m_window_dimension.x),
                         clamp(mousePoint.y, 0, m_window_dimension.y));
  m_input.m_mouse_normalized = vec2(clamp(mousePoint.x / m_window_dimension.x, 0.f, 1.f),
                                    clamp(mousePoint.y / m_window_dimension.y, 0.f, 1.f));
}
-(void)mouseMoved:(NSEvent *)event
{
  [self updateMouse:event];
}

-(void)mouseDragged:(NSEvent *)event
{
  [self updateMouse:event];
}

-(void)updateAndDrawDemoView:(int)width Height:(int)height
{
  m_window_dimension = vec2(width, height);
  
#if 0
  printf("mouse: %.3f %.3f (%.3f, %.3f) (%c%c)\n", m_input.m_mouse.x, m_input.m_mouse.y,
         m_input.m_mouse_normalized.x, m_input.m_mouse_normalized.y,
         m_input.IsDown(m_input.m_mouse_left) ? 'l' : '-', m_input.IsDown(m_input.m_mouse_right) ? 'r' : '-');
#endif
  
  vec3 camera_forward = m_camera_forward;
  // camera movement
  {
    if(m_input.m_mouse_left == DeviceInput::ButtonState::kPressed) {
      m_mouse_drag_start = m_input.m_mouse_normalized;
    }
    
    if(m_mouse_drag_start != vec2::kZero) {
      const float full_rotation = M_PI;
      const vec2 diff = m_input.m_mouse_normalized - m_mouse_drag_start;
      const float yaw_angle = -diff.x * full_rotation;
      const float pitch_angle = -diff.y * full_rotation;
      
      // todo: this suffers from gimbal lock
      const vec3 right = vec3::cross(camera_forward, vec3::kUnitY);
      const mat4 yaw = mat4::rotation(vec3::kUnitY, yaw_angle);
      const mat4 pitch = mat4::rotation(right, pitch_angle);
      const mat4 roll = mat4::kIdentity;
      
      printf("pitch %f, yaw %f\n", pitch_angle, yaw_angle);
      
      const mat4 rotation = pitch * mat4::kIdentity * yaw;
      camera_forward = rotation * m_camera_forward;
    }
    
    if(m_input.m_mouse_left == DeviceInput::ButtonState::kReleased) {
      const vec2 end = m_input.m_mouse_normalized;
      const vec2 diff = end - m_mouse_drag_start;
      m_camera_forward = camera_forward;
      m_mouse_drag_start = vec2::kZero;
      printf("from %3f, %3f to %3f, %3f is %3f %3f\n", m_mouse_drag_start.x, m_mouse_drag_start.y, end.x, end.y, diff.x, diff.y);
    }
    
    const float cam_factor = 0.1f;
    const vec3 camera_right = vec3::cross(camera_forward, vec3::kUnitY);
    if(m_input.IsDown(m_input.m_arrow_up)) m_camera_position += camera_forward * cam_factor;
    if(m_input.IsDown(m_input.m_arrow_down)) m_camera_position -= camera_forward * cam_factor;
    if(m_input.IsDown(m_input.m_arrow_right)) m_camera_position += camera_right * cam_factor;
    if(m_input.IsDown(m_input.m_arrow_left)) m_camera_position -= camera_right * cam_factor;
  }
  
  Input input = create_input(m_input);
  m_input.PostUpdate();
  if(input.IsAnyDown()) {
    m_game->UpdateInput(input);
  }
  m_game->Update(16.f);
  m_renderer->BeginScene(m_window_dimension.x, m_window_dimension.y);
  m_renderer->RenderWorld(m_camera_position, camera_forward, m_game->GetCurrentState());
  m_renderer->EndScene();

  [[self openGLContext] makeCurrentContext];
  [[self openGLContext] flushBuffer];

  if (!animationTimer)
    animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
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
  animationTimer = nil;
  delete m_renderer;
  delete m_game;
  [super dealloc];
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
