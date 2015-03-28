import java.util.Arrays;
import java.util.Comparator;

class AnimalTest{
	public AnimalTest() {
		System.out.println("AnimalTest");	
	} 
    int testCnt;
}
 
class Animal{
    int size;
}
 
class Dog extends Animal{
	private AnimalTest test;
    public Dog(int s){
        size = s;
        System.out.println("before test = " + test);
        test = new AnimalTest();
        System.out.println("after test = " + test);
    }
}
 
class Cat extends Animal{
    public Cat(int s){
        size  = s;
    }
}
 
class AnimalSizeComparator implements Comparator<Animal>{
 
    @Override
    public int compare(Animal o1, Animal o2) {
        return o1.size - o2.size;
    }
    //in this way, all sub classes of Animal can use this comparator.
}
 
public class ArraySortTest {
	static Dog d;				
	
    public static void main(String[] args) {
        Dog d1 = new Dog(2);
        Dog d2 = new Dog(1);
        Dog d3 = new Dog(3);
		
		System.out.println("d = " + d);			//未初始化，为null
		System.out.println("dogArray");
        Dog[] dogArray = {d1, d2, d3};
        printDogs(dogArray);
 
        Arrays.sort(dogArray, new AnimalSizeComparator()); 
        printDogs(dogArray);
 
        System.out.println();
 
        //when you have an array of Cat, same Comparator can be used.
        Cat c1 = new Cat(2);
        Cat c2 = new Cat(1);
        Cat c3 = new Cat(3);
 
        System.out.println("catArray");
        Cat[] catArray = {c1, c2, c3};
        printDogs(catArray);
 
        Arrays.sort(catArray, new AnimalSizeComparator()); 
        printDogs(catArray);
    }
 
    public static void printDogs(Animal[] animals){
        for(Animal a: animals)
            System.out.print("size="+a.size + " ");
        System.out.println();
    }
}
