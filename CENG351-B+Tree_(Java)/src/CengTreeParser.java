import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;


//import org.apache.commons.cli.*; //added
import java.util.Scanner; //added

public class CengTreeParser
{
    public static ArrayList<CengBook> parseBooksFromFile(String filename)
    {
        ArrayList<CengBook> bookList = new ArrayList<CengBook>();
        try{
            Scanner scanner = new Scanner(new File(filename));
            while(scanner.hasNext()){
                String[] book = scanner.nextLine().split("\\|");
                Integer bookId = Integer.parseInt(book[0]);
                String bookTitle = book[1];
                String author = book[2];
                String genre = book[3];
                CengBook newBook = new CengBook(bookId, bookTitle, author, genre);
                bookList.add(newBook);
            }
        }
        catch (FileNotFoundException e){
            // ?
        }
        
        // You need to parse the input file in order to use GUI tables.
        // TODO: Parse the input file, and convert them into CengBooks

        return bookList;
    }

    public static void startParsingCommandLine() throws IOException
    {
        // TODO: Start listening and parsing command line -System.in-.
        // There are 4 commands:
        // 1) quit : End the app, gracefully. Print nothing, call nothing, just break off your command line loop.
        // 2) add : Parse and create the book, and call CengBookRunner.addBook(newlyCreatedBook).
        // 3) search : Parse the bookID, and call CengBookRunner.searchBook(bookID).
        // 4) print : Print the whole tree, call CengBookRunner.printTree().

        // Commands (quit, add, search, print) are case-insensitive.

        Scanner scanner = new Scanner(System.in);
        
        while (true){
            
            String command = scanner.nextLine(); //.trim(); //trim boşlukları kaldırıyormuş, gerek yok galiba
            String[] parts = command.split("\\|");
            if(parts[0].equals("quit")){
                break;
            }
            else if(parts[0].equals("add")){
                Integer bookId = Integer.parseInt(parts[1]);
                String bookTitle = parts[2];
                String author = parts[3];
                String genre = parts[4];
                CengBook newBook = new CengBook(bookId, bookTitle, author, genre);
               // now, add the book
                CengBookRunner.addBook(newBook);
            }
            else if(parts[0].equals("search")){
                Integer searchedBookId = Integer.parseInt(parts[1]);
                // now, search he boook
                CengBookRunner.searchBook(searchedBookId);
            }
            else if(parts[0].equals("print")){
                // print the whole tree, dfs
                CengBookRunner.printTree();
            }
            else{
                continue;
            }
        }
       // Options options = new Options();
        
    }
}
