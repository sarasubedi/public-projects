import java.util.*;

public class HashMap<K extends Comparable<K>, V> implements Map<K,V> {

	private List<List<Entry<K,V>>> 	table;
	private int	count;
	private int  tableSize;

	// For Part III
	private long getLoops;
	private long putLoops;
	

	public HashMap() {
		tableSize = 1000003; // prime number
		table = new ArrayList<List<Entry<K,V>>>(tableSize);

		// initializes table as a list of empty lists
		for (int i = 0; i < tableSize; i++) {
			table.add(new LinkedList<Entry<K,V>>());
		}

		count = 0;

		// For Part III:
		resetGetLoops();
		resetPutLoops();
	}

	// For Part III
	public long getGetLoopCount() {
		return getLoops;
	}

	// For Part III
	public long getPutLoopCount() {
		return putLoops;
	}

	// For Part III
	public void resetGetLoops() {
		getLoops = 0;
	}
	
	// For Part III
	public void resetPutLoops() {
		putLoops = 0;
	}

	public boolean containsKey(K key) {
		// gets the index in the table this key should be in
		int index = Math.abs(key.hashCode()) % tableSize;

		// TODO: complete the rest of this method


		//gets the list at specified index in table
		List<Entry<K,V>> list = table.get(index);
		// gets an Iterator that can iterate over the above list
		Iterator<Entry<K,V>> iter = list.iterator();

		while(iter.hasNext()) {
			Entry<K,V> temp = iter.next();
			if(temp.getKey().equals(key)){
				return true;
			}
		}

		return false; // so it compiles
	}

	public V get (K key) throws KeyNotFoundException {
		// gets the index in the table this key should be in
		int index = Math.abs(key.hashCode()) % tableSize;

		// TODO: complete the rest of this method


		//gets the list at specified index in table
		List<Entry<K,V>> list = table.get(index);
		// gets an Iterator that can iterate over the above list
		Iterator<Entry<K,V>> iter = list.iterator();

		while(iter.hasNext()) {
			getLoops++;
			Entry<K,V> temp = iter.next();
			if(temp.getKey().equals(key)){
				return temp.getValue();
			}
		}

		throw new KeyNotFoundException();
	}


	public List<Entry<K,V> >	entryList() {
		List <Entry<K,V>> resultList = new LinkedList<Entry<K,V>>();
		// TODO: complete the rest of this method

		// Tip: you will need to iterate through each index in the table (and each index holds a list)
		//      you will THEN need to iterate through each element in the linked list chain at a 
		//      specific index and add each element to l

		Iterator<List<Entry<K,V>>> iter = table.iterator();
		while(iter.hasNext()) {

			//gets the list at specified index in table
			List<Entry<K,V>> list = iter.next();
			//System.out.println(list.toString());
			// gets an Iterator that can iterate over the above list
			Iterator<Entry<K,V>> subList = list.iterator();

			while(subList.hasNext()) {
				Entry<K,V> temp = subList.next();
				resultList.add(temp);
			}
		}

		return resultList;
	}
	
	public void put (K key, V value){
		// gets the index in the table this key should be in
		int index = Math.abs(key.hashCode())%tableSize;

		// TODO: complete the rest of this method

		// if key is found, update value.  if key is not found add a new Entry with key,value
		// The tester expects that you will add the newest Entry to the END of the list

		//gets the list at specified index in table
		List<Entry<K,V>> list = table.get(index);
		//System.out.println(list.toString());
		// gets an Iterator that can iterate over the above list
		Iterator<Entry<K,V>> iter = list.iterator();

		while(iter.hasNext()) {
			putLoops++;
			Entry<K,V> temp = iter.next();
			if(temp.getKey().equals(key)){
				temp.setValue(value);
				return;
			}
		}
		Entry<K,V> add = new Entry<K,V>(key, value);
		list.add(add);
		count++;

	}

	public int size() {
		return count;
	}

    public void clear() {
		for(int i = 0; i < tableSize; i++) {
			table.get(i).clear();
		}
        count = 0;
    }

}