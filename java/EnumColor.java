/*
  Output:
	RED is : 红色
	GREEN is : 绿色
	BLUE is : 蓝色
	YELLOW is : 黄色
	BLACK is : 黑色
	WHITE is : 白色
 */
public class EnumColor {
	public static void main(String[] args) {
		for (Color color : Color.values())
			System.out.println(color + " is : " + color.getDesc());
	}

	private enum Color {
		RED("红色"), GREEN("绿色"), BLUE("蓝色"), YELLOW("黄色"), BLACK("黑色"), WHITE("白色");

		private final String desc;

		private Color(String desc) {
			this.desc = desc;
		}

		public String getDesc() {
			return desc;
		}
	}
}
