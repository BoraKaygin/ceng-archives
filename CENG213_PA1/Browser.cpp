#include "Browser.h"

Browser::Browser() {
    windows.append(Window());
}

void Browser::newWindow() {
    // TODO
    windows.prepend(Window());
}

void Browser::closeWindow() {
    // TODO
    if (windows.isEmpty()) return;
    windows.removeNodeAtIndex(0);
}

void Browser::switchToWindow(int index) {
    // TODO
    windows.moveToIndex(index,0);
}

Window &Browser::getWindow(int index) {
    return windows.getNodeAtIndex(index) -> data;
}

void Browser::moveTab(Window &from, Window &to) {
    // TODO
    //Node<Tab> copiedNode(from.getActiveTab());
    //to.addTab(copiedNode);
    to.newTab(from.getActiveTab());
    from.closeTab();
}

void Browser::mergeWindows(Window &window1, Window &window2) {
    // TODO
    if (window2.isEmpty()) return;
    window2.changeActiveTabTo(0);
    while (not window2.isEmpty())
    {
        moveTab(window2,window1);
    }
}

void Browser::mergeAllWindows() {
    // TODO
    if (windows.isEmpty() || windows.getSize() == 1) return;
    Node<Window> *firstPtr = windows.getFirstNode();
    Node<Window> *currentPtr = firstPtr->next;
    while (currentPtr != firstPtr)
    {
        mergeWindows(firstPtr->data,currentPtr->data);
        currentPtr = currentPtr->next;
    }
}

void Browser::closeAllWindows() {
    // TODO
    while (not windows.isEmpty())
    {
        closeWindow();
    }
}

void Browser::closeEmptyWindows() {
    // TODO
    while (not windows.isEmpty() && windows.getFirstNode()->data.isEmpty())
    {
        closeWindow();
    }
    if (windows.isEmpty()) return;
    Node<Window> *first = windows.getFirstNode();
    Node<Window> *current = first->next;
    int index = 1;
    while (current != first)
    {
        if (current->data.isEmpty())
        {
            current = current->next;
            switchToWindow(index);
            closeWindow();
            if (windows.isEmpty()) return;
            continue;
        }
        current = current->next;
        index++;
    }
}

void Browser::print() {
    Node<Window> *head = windows.getFirstNode();
    if(head == NULL) {
        std::cout << "The browser is empty" << std::endl;
    } else {
        (head -> data).print();
        head = head -> next;
        while(head != windows.getFirstNode()) {
            (head -> data).print();
            head = head -> next;
        }
    }
}
