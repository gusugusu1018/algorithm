using System;
using System.IO;

namespace bubblesort
{
		class MainClass
		{
				public static void Main() 
				{
						int start,end,tmp;
						IOarray data = new IOarray();
						data.inputfile();

						for (start = 1; start < data.n; start++)
						{
								for (end = data.n-1; end >= start; end--)
								{
										if (data.nums[end - 1] > data.nums[end])
										{
												tmp = data.nums[end - 1];
												data.nums[end - 1] = data.nums[end];
												data.nums[end] = tmp;
										}
								}
						}
						data.outputfile();
				}
		}

		class IOarray
		{
				public int[] nums = new int[32];
				private int i;
				public int n;
				public void inputfile ()
				{
						try 
						{
								using (StreamReader sr = new StreamReader("data.txt")) 
								{
										string Line;
										while((Line=sr.ReadLine()) != null)
										{
												nums[n]=int.Parse(Line);
												n++;
										}
										Console.WriteLine("---input---");
										for (i=0; i<n; i++){
												Console.WriteLine(nums[i]);
										}
								}
						}
						catch (Exception e) 
						{
								Console.WriteLine("The file could not be read:");
								Console.WriteLine(e.Message);
						}
				}
				public void outputfile ()
				{
						Console.WriteLine("---output---");
						for (i=0; i<n; i++){
								Console.WriteLine(nums[i]);
						}
				}
		}
}

