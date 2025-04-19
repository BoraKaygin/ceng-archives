import java.util.ArrayList;

public class CengTreeNodeLeaf extends CengTreeNode
{
    private ArrayList<CengBook> books; // getBook yzman lazım, addBook da galiba ve eklediğinde yine sorted olarak uygun yere eklemesi lazım
    // TODO: Any extra attributes

    public ArrayList<CengBook> getBooks(){
        return books;
    }

    public CengTreeNodeLeaf(CengTreeNode parent)
    {
        super(parent);

        // TODO: Extra initializations
        type = CengNodeType.Leaf;
        books = new ArrayList<>();
    }

    // GUI Methods - Do not modify
    public int bookCount()
    {
        return books.size();
    }
    public Integer bookKeyAtIndex(Integer index)
    {
        if(index >= this.bookCount()) {
            return -1;
        } else {
            CengBook book = this.books.get(index);

            return book.getBookID();
        }
    }

    public CengBook realBookKeyAtIndex(Integer index)
    {
        if(index >= this.bookCount()) {
            return null;
        } else {
            CengBook book = this.books.get(index);

            return book;
        }
    }

    // Extra Functions
}
