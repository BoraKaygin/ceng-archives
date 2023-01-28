#include "Window.h"

Window::Window() {
    this->activeTab = 0;
    tabs.append(Tab());
}

Tab Window::getActiveTab() {
    // TODO
    if (tabs.isEmpty()) return Tab();
    return tabs.getNodeAtIndex(activeTab)->data;
}

bool Window::isEmpty() const {
    // TODO
    if (tabs.getFirstNode()) return false;
    else return true;
}

void Window::newTab(const Tab &tab) {
    // TODO
    if (tabs.isEmpty())
    {
        tabs.append(tab);
        activeTab++;
        return;
    }
    tabs.insertAtIndex(tab,activeTab+1);
    activeTab++;
}

void Window::closeTab() {
    // TODO
    if (tabs.isEmpty()) return;
    if (activeTab == tabs.getSize()-1)
    {
        tabs.removeNodeAtIndex(activeTab);
        activeTab--;
        return;
    }
    tabs.removeNodeAtIndex(activeTab);
}

void Window::moveActiveTabTo(int index) {
    // TODO
    if (tabs.isEmpty() || index < 0) return;
    if (index > tabs.getSize()-1) index = tabs.getSize()-1;
    tabs.moveToIndex(activeTab,index);
    activeTab = index;
}

void Window::changeActiveTabTo(int index) {
    // TODO
    if (tabs.isEmpty() || index < 0 || index >= tabs.getSize()) return;
    activeTab = index;
}

void Window::addTab(Node<Tab> &tab) {
    // TODO
    if (tabs.isEmpty()) activeTab++;
    tabs.append(tab.data);
}

void Window::print() {
    if(tabs.isEmpty()) std::cout << "Window is empty" << std::endl;
    else std::cout << "The active tab is tab " << activeTab << std::endl;
    tabs.print();
}
