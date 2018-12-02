//
//  main.h
//  OrderSet
//
//  Created by Sam Ng on 12/1/18.
//  Copyright © 2018 Sam Ng. All rights reserved.
//

#ifndef main_h
#define main_h
#include <set>
#include <unordered_map>
#include <string>
#include <vector>


struct Order
{
    Order(int a, int b, double c, std::string d)
    : ref{a}, price{b}, qty{c}, instrument{d} {}
    
    int ref;
    int price;
    double qty;
    std::string instrument;
    
    template<typename T>
    friend T& operator<<(T& t, Order& o)
    {
        t << "ref[" << o.ref << "] price[" << o.price << "] "
        << "qty[" << o.qty << "] instrument[" << o.instrument << "]";
        return t;
    }
};

struct C1
{
  bool operator()(const Order* a, const Order* b) const
    {
        return a->ref < b->ref;
    }
};

struct C2
{
    bool operator()(const Order* a, const Order* b) const
    {
        return a->ref > b->ref;
    }
};

class InstrumentOrderMapBase
{
public:
    virtual ~InstrumentOrderMapBase() = default;
    virtual void Insert(std::string instrument, Order* order) = 0;
    virtual void Update(std::string instrument, Order* order) = 0;
    virtual void Remove(std::string instrument, Order* order) = 0;
    virtual void IterateOrderForInstrument(const std::string& instrument, std::function<void(Order&)>&& handler) = 0;
    virtual void IterateInstrumentOrderMap(std::function<void(const std::string, Order&)>&& handler) = 0;
};

template <typename T>
class InstrumentOrderMap : public InstrumentOrderMapBase
{
public:
    void Insert(std::string instrument, Order* order) override {m_map[instrument].insert(order);}
    void Update(std::string instrument, Order* order) override {m_map[instrument].erase(order); m_map[instrument].insert(order);}
    void Remove(std::string instrument, Order* order) override {m_map[instrument].erase(order);}
    void IterateOrderForInstrument(const std::string& instrument, std::function<void(Order&)>&& handler) override
    {
        for (auto* order : m_map[instrument])
        {
            handler(*order);
        }
    }
    void IterateInstrumentOrderMap(std::function<void(const std::string, Order&)>&& handler) override
    {
        for (auto& item : m_map)
        {
            for (auto* order : item.second)
            {
                handler(item.first, *order);
            }
        }
    }
private:
    std::unordered_map<std::string, std::set<Order*, T>> m_map;
};

class OrderBook
{
public:
    OrderBook() //: mapList{new InstrumentOrderMap<C1>(), new InstrumentOrderMap<C2>()}
    {
        mapList.push_back(new InstrumentOrderMap<C1>());
        mapList.push_back(new InstrumentOrderMap<C2>());
    }
    void Update(Order* order)
    {
        auto* from = table[order];
        auto* to = (order->qty > 100? mapList[0] : mapList[1]);
        table[order] = to;
        Update(order, from, to);
    }
                
    void Update(Order* order, InstrumentOrderMapBase* from, InstrumentOrderMapBase* to)
    {
        if (from == to && from != nullptr)
        {
            from->Update(order->instrument, order);
            return;
        }
        if (from)
            from->Remove(order->instrument, order);
        if (to)
            to->Insert(order->instrument, order);
    }
    
    void IterateBook1ForInstrument(const std::string& instrument, std::function<void(Order&)>&& handler)
    {
        mapList[0]->IterateOrderForInstrument(instrument, std::forward<std::function<void(Order&)>>(handler));
    }
    void IterateBook2ForInstrument(const std::string& instrument, std::function<void(Order&)>&& handler)
    {
        mapList[1]->IterateOrderForInstrument(instrument, std::forward<std::function<void(Order&)>>(handler));
    }
private:
    std::unordered_map<Order*, InstrumentOrderMapBase*> table;
    std::vector<InstrumentOrderMapBase*> mapList;
};


#endif /* main_h */
