using System;
using System.IO;
using System.Collections.Generic;

namespace bubblesort
{
		class MainClass
		{
				public static List<int> data = new List<int>();
				public static void Main() 
				{
						inputdata();
						bubblesort();
						outputdata();
				}
				public static void bubblesort()
				{
						int start,end,tmp;
						for (start = 1; start < data.Count; start++)
						{
								for (end = data.Count-1; end >= start; end--)
								{
										if (data[end - 1] > data[end])
										{
												tmp = data[end - 1];
												data[end - 1] = data[end];
												data[end] = tmp;
										}
								}
						}
				}
				public static void inputdata()
				{
						try 
						{
								using (StreamReader sr = new StreamReader("data.txt")) 
								{
										string Line;
										while((Line=sr.ReadLine()) != null)
										{
												data.Add(int.Parse(Line));
										}
										Console.WriteLine("---inputi data---");
										Console.WriteLine(string.Join(",", data));
								}
						}
						catch (Exception e) 
						{
								Console.WriteLine("The file could not be read:");
								Console.WriteLine(e.Message);
						}
				}
				public static void outputdata()
				{
						Console.WriteLine("---output data---");
						Console.WriteLine(string.Join(",", data));
				}
		}
}
