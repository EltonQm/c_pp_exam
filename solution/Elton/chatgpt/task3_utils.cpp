#include "task3_utils.hpp"

#include <unordered_set>
#include <cctype>
#include <algorithm>

GOStats::GOStats(const std::vector<std::string>& lines)
    : data(lines) {}

/* xrefsearch:
 * For each [Term] block that contains at least one "xref: Reactome:" line,
 * collect:
 *   - its GO id
 *   - all Reactome IDs (R-HSA- or other Reactome identifiers) stripped of "Reactome:"
 *   - all EC numbers (lines "xref: EC:<number>")
 * Output one record per qualifying term.
 */
std::vector<XRefRecord> GOStats::xrefsearch() const {
    std::vector<XRefRecord> results;

    std::string current_id;
    std::unordered_set<std::string> reactome_set;
    std::unordered_set<std::string> ec_set;
    bool in_term = false;

    auto flush = [&]() {
        if (!current_id.empty() && !reactome_set.empty()) {
            XRefRecord rec;
            rec.go_id = current_id;
            rec.reactome_ids.assign(reactome_set.begin(), reactome_set.end());
            rec.ec_ids.assign(ec_set.begin(), ec_set.end());
            // Optional: stable ordering
            std::sort(rec.reactome_ids.begin(), rec.reactome_ids.end());
            std::sort(rec.ec_ids.begin(), rec.ec_ids.end());
            results.push_back(std::move(rec));
        }
        current_id.clear();
        reactome_set.clear();
        ec_set.clear();
    };

    for (const auto &line : data) {
        if (line == "[Term]") {
            flush();
            in_term = true;
        } else if (line.empty()) {
            flush();
            in_term = false;
        } else if (in_term) {
            if (line.rfind("id: GO:", 0) == 0) {
                current_id = line.substr(4); // after "id: "
            } else if (line.rfind("xref: Reactome:", 0) == 0) {
                // Format: xref: Reactome:R-HSA-12345 ! optional text
                std::string token = line.substr(6);               // after "xref: "
                token = token.substr(0, token.find(' '));         // "Reactome:R-HSA-12345"
                auto pos = token.find(':');
                if (pos != std::string::npos) {
                    reactome_set.insert(token.substr(pos + 1));   // keep "R-HSA-12345"
                }
            } else if (line.rfind("xref: EC:", 0) == 0) {
                // Format: xref: EC:1.2.3.4 ! optional text
                std::string token = line.substr(6);
                token = token.substr(0, token.find(' '));         // "EC:1.2.3.4"
                auto pos = token.find(':');
                if (pos != std::string::npos) {
                    ec_set.insert(token.substr(pos + 1));         // "1.2.3.4"
                }
            }
        }
    }
    flush();
    return results;
}

/* obsoleteStats (partitioned):
 * For each obsolete term (is_obsolete: true):
 *   - If it has any replaced_by: lines -> count in with_replaced_by
 *   - Else if it has any consider: lines -> count in with_consider
 *   - Else -> no_alternative
 * total_obsolete counts all obsolete terms.
 */
ObsoleteStats GOStats::obsoleteStats() const {
    ObsoleteStats stats;

    bool in_term = false;
    bool is_obsolete = false;
    bool has_consider = false;
    bool has_replaced = false;

    auto flush = [&]() {
        if (is_obsolete) {
            ++stats.total_obsolete;
            if (has_replaced) {
                ++stats.with_replaced_by;
            } else if (has_consider) {
                ++stats.with_consider;
            } else {
                ++stats.no_alternative;
            }
        }
        is_obsolete = false;
        has_consider = false;
        has_replaced = false;
    };

    for (const auto &line : data) {
        if (line == "[Term]") {
            flush();
            in_term = true;
        } else if (line.empty()) {
            flush();
            in_term = false;
        } else if (in_term) {
            if (line.rfind("is_obsolete: true", 0) == 0) {
                is_obsolete = true;
            } else if (line.rfind("consider:", 0) == 0) {
                has_consider = true;
            } else if (line.rfind("replaced_by:", 0) == 0) {
                has_replaced = true;
            }
        }
    }
    flush();
    return stats;
}

/* hierarchical_relationships:
 * Capture "is_a" and "relationship: part_of" edges.
 * For each term keep the current "id: GO:..." and append lines:
 *   "<child> is_a <parent>"
 *   "<child> part_of <parent>"
 */
std::vector<std::string> GOStats::hierarchical_relationships() const {
    std::vector<std::string> rels;
    std::string current_id;

    for (const auto &line : data) {
        if (line.rfind("id: GO:", 0) == 0) {
            current_id = line.substr(4);
        } else if (line.rfind("is_a: GO:", 0) == 0) {
            if (!current_id.empty()) {
                std::string after = line.substr(6); // after "is_a: "
                std::string target = after.substr(0, after.find(' '));
                rels.emplace_back(current_id + " is_a " + target);
            }
        } else if (line.rfind("relationship: part_of GO:", 0) == 0) {
            if (!current_id.empty()) {
                // e.g. "relationship: part_of GO:0008150 ! comment"
                auto pos = line.find("GO:");
                if (pos != std::string::npos) {
                    std::string target = line.substr(pos);
                    target = target.substr(0, target.find(' '));
                    rels.emplace_back(current_id + " part_of " + target);
                }
            }
        }
    }
    return rels;
}
