using System;
using System.Linq;

namespace Codeforce
{
    class Codeforce1A
    {
        public static int SafeParse(string s){
			int result;
			if( int.TryParse(s, out result) ){
				return result;
			}
			else{
				return 0;
			}
		}
        
        public static void Main(string[] args){
            int[] input = Console.ReadLine().Split(new char[]{ ' ' }).Select( (x) => SafeParse(x) ).ToArray();
            for( int i = 0 ; i < input.Length ; i++ ){
                Console.WriteLine(input[i]);
            }
        }
    }
    
}