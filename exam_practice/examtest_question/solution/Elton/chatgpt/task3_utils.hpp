#ifndef TASK3_UTILS_HPP
#define TASK3_UTILS_HPP

#include <string>
#include <vector>

struct XRefRecord {
    std::string go_id;
    std::vector<std::string> reactome_ids; // Without leading "Reactome:"
    std::vector<std::string> ec_ids;       // EC numbers (e.g., "1.2.3.4")
};

struct ObsoleteStats {
    int total_obsolete = 0;
    int with_consider = 0;      // (only consider, no replaced_by)
    int with_replaced_by = 0;   // (any replaced_by; may also have consider, but we bucket here)
    int no_alternative = 0;     // obsolete with neither consider nor replaced_by
};

class GOStats {
public:
    explicit GOStats(const std::vector<std::string>& lines);

    // List GO terms having at least one Reactome xref (with their Reactome and EC xrefs)
    std::vector<XRefRecord> xrefsearch() const;

    // Obsolete statistics (partitioned categories)
    ObsoleteStats obsoleteStats() const;

    // Hierarchical relationships: "GO:xxx is_a GO:yyy" / "GO:xxx part_of GO:zzz"
    std::vector<std::string> hierarchical_relationships() const;

private:
    const std::vector<std::string>& data;
};


#endif // TASK3_UTILS_HPP
