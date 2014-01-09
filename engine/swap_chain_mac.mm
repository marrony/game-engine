#include "swap_chain.h"

#import <Cocoa/Cocoa.h>

////////////////////////////////////////////////////////////////////////////////////////////

@interface CustomOpenGLView : NSView
{
@private
	NSOpenGLContext* context;
	//NSOpenGLPixelFormat* pixelFormat;
}

- (id) initWithFrame: (NSRect) frame;

- (void) lockFocus;

- (void) swapBuffers;

@end

@implementation CustomOpenGLView

- (id) initWithFrame: (NSRect) frame {
	NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFAWindow,
        NSOpenGLPFASingleRenderer,
        NSOpenGLPFANoRecovery,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)32,
		0
	};

	NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: attrs];

	context = [[NSOpenGLContext alloc] initWithFormat: [pixelFormat autorelease]
										shareContext: nil];

	self = [super initWithFrame: frame];

	return self;
}

- (void) lockFocus {
	[super lockFocus];

	if([context view] != self)
		[context setView: self];

	[context makeCurrentContext];
}

- (void) swapBuffers {
	[context flushBuffer];
}

@end

////////////////////////////////////////////////////////////////////////////////////////////

@interface AppDelegate : NSObject
@end

@implementation AppDelegate

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *)theApplication {
	return YES;
}

@end

extern "C" void swap_chain_create(SwapChain* swap_chain, WindowID parent, int width, int height) {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	[NSApplication sharedApplication];

	AppDelegate *appDelegate = [[AppDelegate alloc] init];
	[NSApp setDelegate: appDelegate];

	[NSApp finishLaunching];

	unsigned int styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;

	NSRect frame = NSMakeRect(0, 0, width, height);
	NSWindow* window  = [[NSWindow alloc] initWithContentRect: frame
	                    styleMask: styleMask
	                    backing: NSBackingStoreBuffered
	                    defer: NO];
	[window setBackgroundColor: [NSColor yellowColor]];
	// [window makeKeyAndOrderFront: NSApp];
	// [window makeKeyAndOrderFront: nil];

	CustomOpenGLView* view = [[CustomOpenGLView alloc] initWithFrame: frame];
	[window setContentView: view];
	[window makeFirstResponder: view];

	[window makeKeyAndOrderFront: NSApp];
	[window makeMainWindow];

	[NSApp activateIgnoringOtherApps: YES];
	
	[pool release];
	
	glewInit();
	
	swap_chain->window = window;
	swap_chain->view = view;
}

extern "C" void swap_chain_destroy(SwapChain* swap_chain) {
	
}

extern "C" void swap_chain_swap_buffers(SwapChain* swap_chain) {
	CustomOpenGLView* view = (CustomOpenGLView*)swap_chain->view;
	[view swapBuffers];
}

extern "C" void swap_chain_process_events(SwapChain* swap_chain) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	do {
		NSEvent* event = [NSApp nextEventMatchingMask: NSAnyEventMask
		                  untilDate: [NSDate distantPast]
		                  inMode: NSDefaultRunLoopMode
		                  dequeue: YES];
		
		if(event == nil)
			break;
		
		[NSApp sendEvent: event];
		[NSApp updateWindows];
		[pool release];
		
		pool = [[NSAutoreleasePool alloc] init];		
	} while(1);

	[pool release];	
}

extern "C" void swap_chain_resize(SwapChain* swap_chain, int width, int height) {
	
}
