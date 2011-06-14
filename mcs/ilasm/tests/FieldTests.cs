// 
// FieldTests.cs
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
	public sealed class FieldTests : AssemblerTester {
		[Test]
		public void TestStaticModuleField ()
		{
			ILAsm ()
				.Input ("field-001.il")
				.Run ()
				.Expect (ExitCode.Success)
				.GetModule ()
				.Expect (x => x.GetType ("<Module>").Fields.Contains (
					y => y.Name == "test001"));
		}
		
		[Test]
		public void TestModuleField ()
		{
			ILAsm ()
				.Input ("field-002.il")
				.ExpectWarning (Warning.GlobalFieldMadeStatic)
				.Run ()
				.Expect (ExitCode.Success)
				.GetModule ()
				.Expect (x => x.GetType ("<Module>").Fields.Contains (
					y => y.IsStatic));
		}
		
		[Test]
		public void TestModuleFieldWithOffset ()
		{
			ILAsm ()
				.Input ("field-003.il")
				.ExpectWarning (Warning.GlobalFieldOffsetIgnored)
				.Run ()
				.Expect (ExitCode.Success)
				.GetModule ()
				.Expect (x => x.GetType ("<Module>").Fields.Contains (
					y => y.Offset == -1));
		}
		
		[Test]
		public void TestClassField ()
		{
			ILAsm ()
				.Input ("field-004.il")
				.Run ()
				.Expect (ExitCode.Success)
				.GetModule ()
				.Expect (x => x.GetType ("test004_cls").Fields.Contains (
					y => y.Name == "test004"));
		}
		
		[Test]
		public void TestClassFieldWithOffset ()
		{
			ILAsm ()
				.Input ("field-005.il")
				.Run ()
				.Expect (ExitCode.Success)
				.GetModule ()
				.Expect (x => x.GetType ("test005_cls").Fields.Contains (
					y => y.Offset == 4));
		}
		
		[Test]
		public void TestMultipleClassFieldsWithOffsets ()
		{
			ILAsm ()
				.Input ("field-006.il")
				.Run ()
				.Expect (ExitCode.Success)
				.GetModule ()
				.Expect (x => x.GetType ("test006_cls").Fields.ContainsMany (
					y => y.Offset == 0,
					y => y.Offset == 2,
					y => y.Offset == 3));
		}
		
		[Test]
		public void TestFieldWithDataLocation ()
		{
			ILAsm ()
				.Input ("field-007.il")
				.ExpectError (Error.InstanceFieldWithDataLocation)
				.Run ()
				.Expect (ExitCode.Error);
		}
	}
}
