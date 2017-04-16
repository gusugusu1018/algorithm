// WARNING
//
// This file has been generated automatically by Xamarin Studio to store outlets and
// actions made in the UI designer. If it is removed, they will be lost.
// Manual changes to this file may not be handled correctly.
//
using Foundation;
using System.CodeDom.Compiler;

namespace bubblesortapp
{
	[Register ("ViewController")]
	partial class ViewController
	{
		[Outlet]
		AppKit.NSTextField inputLabel { get; set; }

		[Outlet]
		AppKit.NSTextField outputLabel { get; set; }

		[Action ("inputbutton:")]
		partial void inputbutton (Foundation.NSObject sender);

		[Action ("outputbutton:")]
		partial void outputbutton (Foundation.NSObject sender);
		
		void ReleaseDesignerOutlets ()
		{
			if (outputLabel != null) {
				outputLabel.Dispose ();
				outputLabel = null;
			}

			if (inputLabel != null) {
				inputLabel.Dispose ();
				inputLabel = null;
			}
		}
	}
}
