#pragma once

#include <queue>
#include "gtl_poly_types.h"

using namespace gtl::operators;

template<typename R>
bool is_ready(std::future<R> const& f)
{ 
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; 
}

struct PolygonSetHelper
{
    PolygonSetHelper() {
        ps.reserve(1000);
    }
    std::queue<std::future<void>> futures;
    PolygonSet ps{};

    void push_future(std::future<void> &&future)
    {
        while (!futures.empty() && is_ready(futures.front())) {
            futures.pop();
        }
        futures.push(std::move(future));
    }

    void wait_futures() 
    {
        while (!futures.empty()) {
            futures.front().wait();
            futures.pop();
        }
    }

    void mergePolygon(Polygon_Holes polygon)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        ps += polygon;
    }

private:
    std::mutex m_mtx;
};