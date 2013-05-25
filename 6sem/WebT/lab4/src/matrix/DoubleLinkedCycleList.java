package matrix;

import java.util.Iterator;

public class DoubleLinkedCycleList implements Iterable<MatrixElement> {
	
	public class DoubleLinkedCycleListIterator implements Iterator<MatrixElement> {
		
		public DoubleLinkedCycleListIterator(ListNode head) {
			this.head = head;
			this.current = head;
			this.isOnCycle = false;
		}

		@Override
		public boolean hasNext() {
			if (isOnCycle)
				return false;
			else
				return true;
		}

		@Override
		public MatrixElement next() {
			if (isOnCycle)
				throw new IndexOutOfBoundsException("Try to go into infinite loop");
			if (current.next == head)
				isOnCycle = true;
			MatrixElement data = current.data;
			current = current.next;
			return data;
		}

		@Override
		public void remove() {
			ListNode prev = current.prev;
			ListNode next = current.next;
			
			prev.next = next;
			next.prev = prev;
		}
		
		private ListNode head;
		private boolean isOnCycle;
		private ListNode current;
	}
	
	private class ListNode {
		public ListNode(ListNode prev, ListNode next, MatrixElement data) {
			this.prev = prev;
			this.next = next;
			this.data = data;
		}
		
		public ListNode prev;
		public ListNode next;
		public MatrixElement data;
	}
	
	public DoubleLinkedCycleList() {
		head = null;
	}
	
	public void add(MatrixElement data) {
		if (head == null) {
			head = new ListNode(null, null, data);
			head.next = head;
			head.prev = head;
		} else {
			ListNode tail = head.prev;
			ListNode ins = new ListNode(tail, head, data);
			tail.next = ins;
			head.prev = ins;
		}
	}
	
    private ListNode head;

	@Override
	public Iterator<MatrixElement> iterator() {
		return new DoubleLinkedCycleListIterator(head);
	}
}
