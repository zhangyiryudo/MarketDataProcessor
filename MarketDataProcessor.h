#include <iostream>
#include <map>
#include <vector>
#include <iomanip>

// Define the type of incoming market data tick
enum class TickType { TRADE, BID, ASK };

struct Tick {
    TickType type;
    double price;
    int volume;
};

class MarketDataProcessor {
private:
    // --- VWAP State ---
    double cumulative_pv;     // Price * Volume
    long long cumulative_vol; // Total Volume

    // --- Order Book State ---
    // Bids: Sorted descending (highest price first)
    std::map<double, int, std::greater<double>> bids;
    
    // Asks: Sorted ascending (lowest price first)
    std::map<double, int> asks;

    // Helper to update the limit order book
    template <typename MapType>
    void update_book(MapType& book, double price, int volume) {
        if (volume <= 0) {
            // Remove the price level if volume is depleted (cancellations/fills)
            book.erase(price); 
        } else {
            // Update or insert the new volume at this price level
            book[price] = volume; 
        }
    }

public:
    MarketDataProcessor() : cumulative_pv(0.0), cumulative_vol(0) {}

    // Process an incoming tick
    void process_tick(const Tick& tick) {
        switch (tick.type) {
            case TickType::TRADE:
                // O(1) Update for VWAP
                cumulative_pv += (tick.price * tick.volume);
                cumulative_vol += tick.volume;
                break;
            case TickType::BID:
                // O(log N) Update for Bids
                update_book(bids, tick.price, tick.volume);
                break;
            case TickType::ASK:
                // O(log N) Update for Asks
                update_book(asks, tick.price, tick.volume);
                break;
        }
    }

    // Get current VWAP: O(1)
    double get_vwap() const {
        if (cumulative_vol == 0) return 0.0;
        return cumulative_pv / cumulative_vol;
    }

    // Print current Top of Book: O(1)
    void print_top_of_book() const {
        double best_bid = bids.empty() ? 0.0 : bids.begin()->first;
        int bid_vol = bids.empty() ? 0 : bids.begin()->second;

        double best_ask = asks.empty() ? 0.0 : asks.begin()->first;
        int ask_vol = asks.empty() ? 0 : asks.begin()->second;

        std::cout << "Top of Book | Bid: " << bid_vol << " @ $" << std::fixed << std::setprecision(2) << best_bid 
                  << "  ---  Ask: " << ask_vol << " @ $" << best_ask << std::endl;
    }
};
