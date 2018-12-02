//
//  main.cpp
//  OrderSet
//
//  Created by Sam Ng on 12/1/18.
//  Copyright © 2018 Sam Ng. All rights reserved.
//

#include <iostream>
#include "main.h"

int main(int argc, const char * argv[]) {
    OrderBook book;
    Order o1{1, 2, 10, "A"};
    Order o2{2, 2, 10, "A"};
    Order o3{3, 2, 10, "A"};
    Order o4{4, 2, 400, "A"};
    
    book.Update(&o1);
    book.Update(&o2);
    book.Update(&o3);
    book.Update(&o4);
    
    auto dump = [](Order& order)
    {
        std::cout << order << std::endl;
    };
    std::cout << "Book1: \n";
    book.IterateBook1ForInstrument("A", dump);
    
    std::cout << "Book2: \n";
    book.IterateBook2ForInstrument("A", dump);
    
    o1.qty = 200;
    o3.qty = 30;
    
    book.Update(&o1);
    book.Update(&o3);
    
    std::cout << "Book1: \n";
    book.IterateBook1ForInstrument("A", dump);
    
    std::cout << "Book2: \n";
    book.IterateBook2ForInstrument("A", dump);
    
    return 0;
}
