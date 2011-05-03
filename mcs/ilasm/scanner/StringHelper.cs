// StringHelper.cs
// Author: Sergey Chaban (serge@wildwestsoftware.com)
using System;
using System.Text;

namespace Mono.ILAsm {
	internal sealed class StringHelper : StringHelperBase {
		private const string startIdChars = "#$@_";
		private const string idChars = "_$@?`";

		public StringHelper (ILTokenizer host)
			: base (host)
		{
		}

		public override bool Start (char ch)
		{
			TokenId = Token.UNKNOWN;

			if (char.IsLetter (ch) || startIdChars.IndexOf (ch) != -1)
				TokenId = Token.ID;
			else if (ch == '\'')
				TokenId = Token.SQSTRING;
			else if (ch == '"')
				TokenId = Token.QSTRING;

			return TokenId != Token.UNKNOWN;
		}

		private static bool IsIdChar (int c)
		{
			char ch = (char) c;
			return (char.IsLetterOrDigit (ch) || idChars.IndexOf (ch) != -1);
		}

		public override string Build ()
		{
			if (TokenId == Token.UNKNOWN)
				return String.Empty;
			
			int ch = 0;
			var reader = host.Reader;
			var idsb = new StringBuilder ();
			
			if (TokenId == Token.SQSTRING || TokenId == Token.QSTRING) {
				int term = (TokenId == Token.SQSTRING) ? '\'' : '"';
				reader.Read (); // skip quote
				for (ch = reader.Read (); ch != -1; ch = reader.Read ()) {
					if (ch == term)
						break;

					if (ch == '\\') {
						ch = reader.Read ();

						/*
							* Long string can be broken across multiple lines
							* by using '\' as the last char in line.
							* Any white space chars between '\' and the first
							* char on the next line are ignored.
							*/
						if (ch == '\n') {
							reader.SkipWhitespace ();
							continue;
						}

						int escaped = Escape (reader, ch);
						if (escaped == -1) {
							reader.Unread (ch);
							ch = '\\';
						} else {
							ch = escaped;
						}
					}

					idsb.Append ((char) ch);
				}
			} else { // ID
				while ((ch = reader.Read ()) != -1) {
					if (IsIdChar (ch)) {
						idsb.Append ((char) ch);
					} else {
						reader.Unread (ch);
						break;
					}
				}
			}
			
			return idsb.ToString ();
		}

		public static int Escape (ILReader reader, int ch)
		{
			int res = -1;

			if (ch >= '0' && ch <= '7') {
				var octal = new StringBuilder ();
				octal.Append ((char) ch);
				int possibleOctalChar = reader.Peek ();
				if (possibleOctalChar >= '0' && possibleOctalChar <= '7') {
					octal.Append ((char) reader.Read ());
					possibleOctalChar = reader.Peek ();
					if (possibleOctalChar >= '0' && possibleOctalChar <= '7')
						octal.Append ((char) reader.Read ());
				}
				
				res = Convert.ToInt32 (octal.ToString (), 8);
			} else {
				int id = "abfnrtv\"'\\".IndexOf ((char) ch);
				if (id != -1)
					res = "\a\b\f\n\r\t\v\"'\\" [id];
			}

			return res;
		}
	}
}
