// fork_join sort using quicksort

import java.util.concurrent.*; 

public class ForkJoinSort extends RecursiveAction {
    /**
     *
     */
    private static final long serialVersionUID = 1L;
    static final int THRESHOLD = 5;
    private int p;
    private int r;
    private int array[];

    public ForkJoinSort(int begin, int end, int array[]) {
        this.p = begin;
        this.r = end;
        this.array = array;
    }

    public void serial_quicksort(int begin, int end) {
        if (begin >= end) {
            return;
        }
        int pivot = array[end];
        int i = begin - 1, j = begin, tmp;
        while (j < end) {
            if (array[j] <= pivot) {
                i++;
                tmp = array[j];
                array[j] = array[i];
                array[i] = tmp;
            }
            j++;
        }
        i++;
        tmp = array[end];
        array[end] = array[i];
        array[i] = tmp;
        serial_quicksort(begin, i - 1);
        serial_quicksort(i + 1, end);
    }

    protected void compute() {
        if (r - p <= THRESHOLD) {
            this.serial_quicksort(p, r);
        } else {
            int pivot = array[r];
            int i = p - 1, j = p, tmp;
            while (j < r) {
                if (array[j] <= pivot) {
                    i++;
                    tmp = array[j];
                    array[j] = array[i];
                    array[i] = tmp;
                }
                j++;
            }
            i++;
            tmp = array[r];
            array[r] = array[i];
            array[i] = tmp;

            ForkJoinSort leftTask = new ForkJoinSort(p, i - 1, array);
            ForkJoinSort rightTask = new ForkJoinSort(i + 1, r, array);

            leftTask.fork();
            rightTask.fork();
            
            leftTask.join();
            rightTask.join();
        }
    }
    public static void main(String[] args) {
        int arr[] = { 3, 4, 5, 62, 6, 763, 7, 57, 4, 7, 47, 4, 3, 8, 684, 58, 45, 6, 456, 848, 64, 58 };
        ForkJoinPool pool = new ForkJoinPool();
        ForkJoinSort st = new ForkJoinSort(0, arr.length - 1, arr);
        pool.invoke(st);
        for (int i : arr) {
            System.out.print(i + " ");
        }
    }
}