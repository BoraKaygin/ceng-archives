import java.util.ArrayList;

public class CengTree
{
    public CengTreeNode root;
    // Any extra attributes...
    public ArrayList<CengTreeNode> searchReturn; // new

    public CengTree(Integer order)
    {
        CengTreeNode.order = order;
        // TODO: Initialize the class
        root = new CengTreeNodeLeaf(null);
    }

    public void addBook(CengBook book)
    {
        // TODO: Insert Book to Tree
        if(root.getType()==CengNodeType.Leaf){
            CengTreeNodeLeaf leaf = (CengTreeNodeLeaf) root;
            ArrayList<CengBook> books = leaf.getBooks();
            int bookCount = leaf.bookCount();

            int i;
            for(i=0; i < bookCount; i++){
                if(book.getBookID() < leaf.bookKeyAtIndex(i)){
                    break;
                }
            }
            books.add(i, book);
            bookCount++;

            if (bookCount > CengTreeNode.order*2){ // leaf node taştı, splitle
                CengTreeNodeInternal new_root = new CengTreeNodeInternal(null);
                ArrayList<Integer> new_root_keys = new_root.getKeys();
                ArrayList<CengTreeNode> children = new_root.getAllChildren();
                CengTreeNodeLeaf leaf1 = new CengTreeNodeLeaf(new_root);
                CengTreeNodeLeaf leaf2 = new CengTreeNodeLeaf(new_root);
                ArrayList<CengBook> books1 = leaf1.getBooks();
                ArrayList<CengBook> books2 = leaf2.getBooks();
                //int i;
                for(i=0; i < CengTreeNode.order; i++){ // order kadarını leaf1'e
                    books1.add(leaf.realBookKeyAtIndex(i));

                }
                for(; i < bookCount; i++){ // geri kalanını leaf2 ye koydum
                    books2.add(leaf.realBookKeyAtIndex(i));
                }
                new_root_keys.add(books2.get(0).getBookID());
                children.add(leaf1);
                children.add(leaf2);
                root = new_root;
            }
        }
        else{
            AddUsingInternalRoot(book, root);

            // child ve kendi için taşma handle la
            // hangi çocuk olduğunu nerden??
            CengTreeNodeInternal rootInternal = (CengTreeNodeInternal) root;
            ArrayList<Integer> keys = rootInternal.getKeys();
            ArrayList<CengTreeNode> children = rootInternal.getAllChildren();
            int keyCount = rootInternal.keyCount();

            int child_num;
            for(child_num=0; child_num<keyCount; child_num++){
                if(book.getBookID() < rootInternal.keyAtIndex((child_num))){
                    break;
                }
            }
            // chilnum: possible overflowu olan çocuk


            //şimdi root için overflow kontrolü yapılacak, root bir internal
            if(rootInternal.keyCount() > CengTreeNode.order*2){ //taşmış
                CengTreeNodeInternal new_root = new CengTreeNodeInternal(null);
                CengTreeNodeInternal rootSplit1 = new CengTreeNodeInternal(root);
                CengTreeNodeInternal rootSplit2 = new CengTreeNodeInternal(root);
                ArrayList<Integer> rootSplit1_keys = rootSplit1.getKeys();
                ArrayList<Integer> rootSplit2_keys = rootSplit2.getKeys();
                ArrayList<CengTreeNode> rootSplit1_children = rootSplit1.getAllChildren();
                ArrayList<CengTreeNode> rootSplit2_children = rootSplit2.getAllChildren();

                ArrayList<CengTreeNode> oldRoot_children = rootInternal.getAllChildren();
                ArrayList<Integer> oldRoot_keys = rootInternal.getKeys();
                int keyToBeUp = oldRoot_keys.get(CengTreeNode.order);
             //   oldRoot_keys.remove(CengTreeNode.order); //child da mı silmem gerek bunu silince?? confusion
                oldRoot_keys.set(CengTreeNode.order, null);


                for(int j=0; j<(CengTreeNode.order*2)+1; j++){ //-1 vardı ortada, kaldırdım
                    if(j<CengTreeNode.order){
                        rootSplit1_keys.add(oldRoot_keys.get(j));
                       // rootSplit1_children.add(oldRoot_children.get(j));
                    } else{
                        if(oldRoot_keys.get(j)!=null)
                            rootSplit2_keys.add(oldRoot_keys.get(j));
                       // rootSplit2_children.add(oldRoot_children.get(j));
                    }
                }
                for(int j=0; j<oldRoot_children.size(); j++){
                    if(j<(CengTreeNode.order)+1){
                        rootSplit1_children.add(oldRoot_children.get(j));
                    }
                    else{
                        rootSplit2_children.add(oldRoot_children.get(j));
                    }
                }
                new_root.getAllChildren().add(rootSplit1); // bağlanmış oldu mu şimdi? eski root da kaldı ama bağlı?
                new_root.getAllChildren().add(rootSplit2);

                new_root.getKeys().add(keyToBeUp);
               // keyCount++; //?? whose keyCount
                root = new_root;
            }


        }
    }
    public void AddUsingInternalRoot(CengBook book, CengTreeNode node){
        if(node.getType() == CengNodeType.Internal){
            CengTreeNodeInternal internal = (CengTreeNodeInternal) node;
            ArrayList<Integer> keys = internal.getKeys();
            ArrayList<CengTreeNode> children = internal.getAllChildren();
            int keyCount = internal.keyCount();
            int i;
            for(i=0; i<=keyCount; i++){
                if(i==keyCount || book.getBookID() < internal.keyAtIndex(i)){ // ya en büyükse pekii?????
                    if(children.get(i).getType()==CengNodeType.Internal){ // child bir internal mış
                        AddUsingInternalRoot(book, children.get(i));
                        //şimdi çocuk internal node taşmış mı diye bakacağız
                        CengTreeNodeInternal child_internal = (CengTreeNodeInternal) children.get(i);
                        if(child_internal.keyCount() > CengTreeNode.order*2){
                            CengTreeNodeInternal new_internal1 = new CengTreeNodeInternal(node); // node yerine children.get(i) yazıyordu, düzelttim
                            CengTreeNodeInternal new_internal2 = new CengTreeNodeInternal(node);
                            ArrayList<Integer> new_internal_keys1 = new_internal1.getKeys();
                            ArrayList<Integer> new_internal_keys2 = new_internal2.getKeys();
                            ArrayList<CengTreeNode> new_internal_children1 = new_internal1.getAllChildren();
                            ArrayList<CengTreeNode> new_internal_children2 = new_internal2.getAllChildren();

                            ArrayList<CengTreeNode> child_children = child_internal.getAllChildren();
                            ArrayList<Integer> child_keys = child_internal.getKeys();
                            int keyToBeUp = child_keys.get(CengTreeNode.order);
                           // child_keys.remove(CengTreeNode.order);
                            child_keys.set(CengTreeNode.order,null);

                            for(int j=0; j<(CengTreeNode.order*2)+1; j++){ //-1 vardı ortada, kaldırdım: zaten 1 fazlaydı taştığı için, yukarda remove edince tam oldu
                                if(j<CengTreeNode.order){
                                    new_internal_keys1.add(child_keys.get(j));
                                    //new_internal_children1.add(child_children.get(j));
                                }else{
                                    if(child_keys.get(j)!=null){
                                        new_internal_keys2.add(child_keys.get(j));
                                       // new_internal_children2.add(child_children.get(j));
                                    }
                                }
                            }
                            for(int j=0; j<child_children.size(); j++){
                                if(j<CengTreeNode.order+1){
                                    new_internal_children1.add(child_children.get(j));
                                }else{
                                    new_internal_children2.add(child_children.get(j));
                                }
                            }
                            children.set(i, new_internal1);
                            children.add(i+1, new_internal2);

                            keys.add(i, keyToBeUp);
                        }
                    }
                    else{ // child bir leafmiş
                        CengTreeNodeLeaf leaf = (CengTreeNodeLeaf) children.get(i);
                        ArrayList<CengBook> books = leaf.getBooks();
                        int bookCount = leaf.bookCount();
                        int j;
                        for(j=0; j < bookCount; j++){
                            if(book.getBookID() < leaf.bookKeyAtIndex(j)){
                                break;
                            }
                        }
                        books.add(j, book);
                        bookCount++;
                        if (bookCount > CengTreeNode.order*2){ // child olan leaf node taştı, splitle
                           // CengTreeNodeInternal new_root = new CengTreeNodeInternal(null);
                            CengTreeNodeLeaf new_leaf1 = new CengTreeNodeLeaf(children.get(i));
                            CengTreeNodeLeaf new_leaf2 = new CengTreeNodeLeaf(children.get(i));
                            ArrayList<CengBook> new_leafBooks1 = new_leaf1.getBooks();
                            ArrayList<CengBook> new_leafBooks2 = new_leaf2.getBooks();
                            for(j=0; j < (CengTreeNode.order*2)+1; j++){ //ordera kadar olanları new_leaf1 e sonrasını 2 ye koydum, +1 yazdım çünkü taşmış bu
                                if(j<CengTreeNode.order){
                                    new_leafBooks1.add(books.get(j));
                                }else{
                                    new_leafBooks2.add(books.get(j));
                                }

                            }
                            //CengTreeNode new_node = (CengTreeNode) new_leaf;
                            children.remove(i); //leafi sildim
                            children.add(i, new_leaf1); //newleaf1 i leafin yerine koydum
                            children.add(i+1, new_leaf2); //newleaf2 yi 1 in sağına koydum

                            int new_key = new_leafBooks2.get(0).getBookID(); // new_key new_leaf2 nin başında
                            keys.add(i, new_key);
                            keyCount++; //  ııııaaaaaa
                        }



                    }
                    break;
                }
            }
        }

    }

    public boolean searchHelper(CengTreeNode node, Integer bookID){
        if(node.getType()==CengNodeType.Leaf){
            CengTreeNodeLeaf leaf = (CengTreeNodeLeaf) node;
            ArrayList<CengBook> books = leaf.getBooks();
            for(int i=0; i<books.size(); i++){
                if(books.get(i).getBookID().equals(bookID)){
                    return true;
                }
            }
            return false;
        }
        else{
            CengTreeNodeInternal internal = (CengTreeNodeInternal) node;
            ArrayList<Integer> keys = internal.getKeys();
            ArrayList<CengTreeNode> children = internal.getAllChildren();
            int i;
            for(i=0; i<keys.size(); i++){
                if(keys.get(i)<=bookID) continue;
                break;
            }
            return searchHelper(children.get(i), bookID);
        }
    }

    public void printPath(CengTreeNode node, int bookID, int depth){
        searchReturn.add(node); //new
        if(node.getType()==CengNodeType.Leaf){
            CengTreeNodeLeaf leaf = (CengTreeNodeLeaf) node;
            ArrayList<CengBook> books = leaf.getBooks();
            for(int i=0; i<books.size(); i++){
                if(books.get(i).getBookID()==bookID){
                    tabPutter(depth);
                    System.out.print("<record>" + books.get(i).fullName() + "</record>" + "\n");
                }
            }
        }
        else{
            CengTreeNodeInternal internal = (CengTreeNodeInternal) node;
            ArrayList<Integer> keys = internal.getKeys();
            ArrayList<CengTreeNode> children = internal.getAllChildren();
            int i;
            for(i=0; i<keys.size(); i++){
                if(keys.get(i)<=bookID) continue;
                break;
            }
            tabPutter(depth);
            System.out.print("<index>" + "\n");
            for(int j=0; j<keys.size(); j++){
                tabPutter(depth);
                System.out.print(keys.get(j) + "\n");
            }
            tabPutter(depth);
            System.out.print("</index>" + "\n");
            printPath(children.get(i), bookID,depth+1);
        }
    }
    public ArrayList<CengTreeNode> searchBook(Integer bookID)
    {
        // TODO: Search within whole Tree, return visited nodes.
        // Return null if not found.
        if(searchHelper(root, bookID)==false){
            System.out.print("Could not find " + bookID + "." + "\n");
            return null;
        }
        else{
            searchReturn = new ArrayList<>(); // new
            printPath(root, bookID, 0);
            return searchReturn; // new
        }
    }

    public void tabPutter(int depth){
        for(int i=0; i<depth; i++){
            System.out.print("\t");
        }
    }
    public void printNode(CengTreeNode node, int depth){
        if(node.getType()==CengNodeType.Leaf){
            tabPutter(depth);
            CengTreeNodeLeaf leaf = (CengTreeNodeLeaf) node;
            ArrayList<CengBook> books = leaf.getBooks();
            System.out.print("<data>");
            System.out.print("\n");
            for(int i=0; i<books.size(); i++){
                tabPutter(depth);
                System.out.print("<record>" + books.get(i).fullName() + "</record>" + "\n");
            }
            tabPutter(depth);
            System.out.print("</data>");
            System.out.print("\n");
        }else{
            tabPutter(depth);
            System.out.print("<index>");
            System.out.print("\n");
            CengTreeNodeInternal internal = (CengTreeNodeInternal) node;
            ArrayList<Integer> keys = internal.getKeys();
            ArrayList<CengTreeNode> children = internal.getAllChildren();
            for(int i=0; i<keys.size(); i++){
                tabPutter(depth);
                System.out.print(keys.get(i) + "\n");
            }
            tabPutter(depth);
            System.out.print("</index>");
            System.out.print("\n");
            for(int i=0; i<children.size(); i++){
                printNode(children.get(i), depth+1);
            }
        }
    }
    public void printTree()
    {
        printNode(root, 0);
        // TODO: Print the whole tree to console
    }

    // Any extra functions...
}
