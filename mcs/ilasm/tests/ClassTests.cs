// 
// ClassTests.cs
//  
// Author:
//       Alex Rønne Petersen <xtzgzorex@gmail.com>
// 
// Copyright (c) 2011 Alex Rønne Petersen
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
using System;
using NUnit.Framework;

namespace Mono.ILAsm.Tests {
	[TestFixture]
	public sealed class ClassTests : AssemblerTester {
		[Test]
		public void TestSimpleClassDirective ()
		{
			ILAsm ()
				.Input ("class-001.il")
				.Run ()
				.Expect (ExitCode.Success)
				.GetModule ()
				.Expect (x => x.GetType ("test001") != null);
		}
		
		[Test]
		public void TestGenericClassDirective ()
		{
			ILAsm ()
				.Input ("class-002.il")
				.Run ()
				.Expect (ExitCode.Success)
				.GetModule ()
				.Expect (x => x.GetTypes ().Contains (
					y => y.GenericParameters.ContainsMany (
						z => z.Name == "T1",
						z => z.Name == "T2")));
		}
	}
}
