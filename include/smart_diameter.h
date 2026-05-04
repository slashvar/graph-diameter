/*
 * Smart diameter: compute the diameter trying to skip useless nodes.
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <deque>
#include <optional>
#include <vector>

#include "bfs.h"
#include "graph.h"

namespace smart {

struct Result
{
    std::size_t diameter;
    std::size_t bfs_runs;
    std::size_t diameter_vertex;
    std::size_t last_change;  // run number at which the diameter last increased
};

class Diameter
{
public:
    explicit Diameter(std::size_t order, std::optional<std::size_t> source = std::nullopt) :
      done_(order, 0),
      scheduled_(order, 0),
      dist_(order),
      bfs_(order),
      order_(order),
      min_excentricity_(order),
      source_(source)
    {
    }

    Result operator()(const Graph& graph)
    {
        if (order_ == 0) {
            return { .diameter = 0, .bfs_runs = 0, .diameter_vertex = 0, .last_change = 0 };
        }

        candidate_.push_back(source_.value_or(lowest_degree_vertex(graph)));

        while (not candidate_.empty() and not found()) {
            const auto v = candidate_.front();
            candidate_.pop_front();
            run_bfs(graph, v);

            // After run_bfs, bfs_.last_visited() refers to the just-completed run.
            // Re-read it (last2) after the inner run rather than reusing `last`.
            const auto last = bfs_.last_visited();
            if (worth_running(last)) {
                run_bfs(graph, last);
                const auto last2 = bfs_.last_visited();
                if (worth_running(last2)) {
                    run_bfs(graph, last2);
                }
            }
            populate();
        }

        return {
            .diameter        = max_excentricity_,
            .bfs_runs        = runs_,
            .diameter_vertex = diam_vertex_,
            .last_change     = last_change_,
        };
    }

private:
    void run_bfs(const Graph& graph, std::size_t v)
    {
        ++runs_;
        done_[v] = 1;
        bfs_.reset();
        bfs_(graph, v);
        if (bfs_.max_distance() > max_excentricity_) {
            max_excentricity_ = bfs_.max_distance();
            last_change_      = runs_;
            diam_vertex_      = v;
        }
        min_excentricity_ = std::min(min_excentricity_, bfs_.max_distance());
    }

    [[nodiscard]] bool worth_running(std::size_t v) const noexcept
    {
        return done_[v] == 0 and bfs_.distance(v) > threshold() and bfs_.distance(v) >= dist_[v];
    }

    [[nodiscard]] std::size_t threshold() const noexcept
    {
        if (min_excentricity_ < max_excentricity_) {
            return min_excentricity_ + ((max_excentricity_ - min_excentricity_) / 2);
        }
        return max_excentricity_ / 2;
    }

    void populate()
    {
        for (std::size_t v = 0; v < order_; ++v) {
            dist_[v] = std::max(bfs_.distance(v), dist_[v]);
            if (done_[v] == 0 and scheduled_[v] == 0 and bfs_.is_leaf(v) and bfs_.distance(v) > threshold()
                and bfs_.distance(v) >= dist_[v]) {
                scheduled_[v] = 1;
                candidate_.push_back(v);
            }
        }
        const auto eliminate = [this](std::size_t v) {
            const bool keep_eliminated = (done_[v] != 0) or not bfs_.is_leaf(v);
            done_[v]                   = keep_eliminated ? char { 1 } : char { 0 };
            return keep_eliminated;
        };
        candidate_.erase(std::ranges::remove_if(candidate_, eliminate).begin(), candidate_.end());
    }

    [[nodiscard]] static std::size_t lowest_degree_vertex(const Graph& graph)
    {
        std::size_t best_vertex = 0;
        std::size_t best_degree = graph[0].size();
        for (std::size_t v = 1; v < graph.order(); ++v) {
            if (graph[v].size() < best_degree) {
                best_degree = graph[v].size();
                best_vertex = v;
            }
        }
        return best_vertex;
    }

    [[nodiscard]] bool found() const noexcept
    {
        return max_excentricity_ >= min_excentricity_ * 2;
    }

    std::deque<std::size_t>    candidate_;
    std::vector<char>          done_;
    std::vector<char>          scheduled_;
    std::vector<std::size_t>   dist_;
    BFS                        bfs_;
    std::size_t                order_;
    std::size_t                max_excentricity_ = 0;
    std::size_t                min_excentricity_;
    std::size_t                runs_        = 0;
    std::size_t                last_change_ = 0;
    std::size_t                diam_vertex_ = 0;
    std::optional<std::size_t> source_;
};

};  // namespace smart
