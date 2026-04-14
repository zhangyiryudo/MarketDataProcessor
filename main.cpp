
#include <iostream>
#include <vector>
#include "MarketDataProcessor.h"

int main() {
    MarketDataProcessor processor;

    // Simulated stream of incoming market ticks
    std::vector<Tick> tick_stream = {
        {TickType::BID, 100.00, 50},   // New bid
        {TickType::ASK, 100.50, 100},  // New ask
        {TickType::BID, 100.10, 20},   // Better bid (New Top)
        {TickType::TRADE, 100.10, 20}, // Trade occurs
        {TickType::BID, 100.10, 0},    // Bid filled/canceled (Volume drops to 0)
        {TickType::TRADE, 100.20, 80}, // Another trade occurs
        {TickType::ASK, 100.40, 50}    // Better ask (New Top)
    };

    std::cout << "--- Processing Market Data Stream ---" << std::endl;
    
    for (const auto& tick : tick_stream) {
        processor.process_tick(tick);
        
        // Output state after each tick
        processor.print_top_of_book();
        std::cout << "Current VWAP: $" << processor.get_vwap() << "\n\n";
    }

    return 0;
}