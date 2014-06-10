/*  
  Output:
	0
	1
	2
	3
	4
	说明ordinal值是固定不变的。
 */
public class EnumMonth{
	public static void main(String[] args) {
		for (Month month : Month.values()) {
			System.out.println(month + ":" + month.ordinal());
		}
	}

	private enum Month {
		JAN(1), SEP(9), MAR(3), APR(4), MAY(5);
		
		final int id;
		
		Month(int id){
			this.id = id;
		}
	}
}
