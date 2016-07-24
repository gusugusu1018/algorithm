using System;
using System.IO;
using System.Collections.Generic;
using AppKit;
using Foundation;

namespace bubblesortapp
{
	public partial class ViewController : NSViewController
	{
		private bool inputbuttonFlag = false;
		//		private bool outputbuttonFlag = false;
		public static List<int> data = new List<int>();

		partial void inputbutton(NSObject sender)
		{
			inputbuttonFlag = true;
			using (StreamReader sr = new StreamReader("data.txt")) 
			{
				for (int i = data.Count + 1 ; i>= 0;i--)
					data.Remove(i);
				string Line;
				while ((Line = sr.ReadLine()) != null)
				{
					data.Add(int.Parse(Line));
				}
				inputLabel.StringValue = string.Join(",", data);
				outputLabel.StringValue = string.Format("Please push the outputbutton");
			}
		}

		partial void outputbutton(NSObject sender)
		{
//			outputbuttonFlag = true;
			if (inputbuttonFlag == false)
			{
				outputLabel.StringValue = string.Format("error");
			}
			else
			{
				int i, j;
				int temp;

				for (i = 0; i < data.Count - 1; i++)
				{
					for (j = data.Count - 1; j > i; j--)
					{
						if (data[j] < data[j - 1])
						{
							temp = data[j - 1];
							data[j - 1] = data[j];
							data[j] = temp;
						}
					}

				}
				outputLabel.StringValue = string.Join(",", data);
			}
		}

		public ViewController(IntPtr handle) : base(handle)
		{
		}

		public override void ViewDidLoad()
		{
			base.ViewDidLoad();

			// Do any additional setup after loading the view.
			inputLabel.StringValue = string.Format("Please push the input button");
			outputLabel.StringValue = string.Format("Don't push the output button");
		}

		public override NSObject RepresentedObject
		{
			get
			{
				return base.RepresentedObject;
			}
			set
			{
				base.RepresentedObject = value;
				// Update the view, if already loaded.
			}
		}
	}
}
