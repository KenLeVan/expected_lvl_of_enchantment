#include <iostream>
#include <random>
#include <map>
#include <string>
#include <vector>

enum class rarity
{
    common,
    uncommon,
    rare,
    epic,
    legendary,
    artefact,
    invalid
};

const std::map<rarity, std::vector<int>> CHANCE_TABLE
{
    {rarity::common, {90, 90, 80, 80, 70, 70, 60, 60, 50, 50}},
    {rarity::uncommon, {90, 80, 80, 70, 70, 60, 60, 50, 50, 40}},
    {rarity::rare, {90, 80, 80, 70, 70, 60, 50, 50, 40, 40}},
    {rarity::epic, {90, 80, 70, 60, 60, 50, 50, 40, 40, 30}},
    {rarity::legendary, {90, 80, 70, 60, 50, 50, 40, 30, 30, 20}},
    {rarity::artefact, {90, 80, 70, 60, 50, 50, 40, 30, 20, 10}}
};

class item
{
private:
    rarity item_rarity;
    int lvl;

    int get_success_chance() const
    {
        const auto& chance = CHANCE_TABLE.at(item_rarity);
        return chance[lvl];
    }
public:
    item(rarity new_rarity = rarity::common, int new_lvl = 0) : item_rarity(new_rarity), lvl(new_lvl) {}
    ~item() {}

    int get_lvl() const { return lvl; };
    rarity get_rarity() const { return item_rarity; };

    void attempt_upgrade()
    {
        if (lvl == 10) { return; }

        int chance = get_success_chance();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, 100);
        int random_value = distrib(gen);

        if (random_value <= chance) { ++lvl; }

        else
        {
            if (lvl > 0)
            {
                --lvl;
            }
        }   
    }
};

rarity string_to_rarity (const std::string& str)
{
    static const std::map<std::string, rarity> rarity_map
    {
        {"common", rarity::common},
        {"uncommon", rarity::uncommon},
        {"rare", rarity::rare},
        {"epic", rarity::epic},
        {"legendary", rarity::legendary},
        {"artefact", rarity::artefact}
    };

    auto it = rarity_map.find(str);
    if (it != rarity_map.end())
    {
        return it->second;
    }
    return rarity::invalid;
}

std::string rarity_to_string (const rarity& r)
{
    switch(r)
    {
        case rarity::common: return "common";
        case rarity::uncommon: return "uncommon";
        case rarity::rare: return "rare";
        case rarity::epic: return "epic";
        case rarity::legendary: return "legendary";
        case rarity::artefact: return "artefact";
        default: return "invalid";
    }
}

void find_expected_lvls_of_enchantments (std::map<int, int>& counts,
                                        const int& number_of_trials,
                                        item& my_item)
{
    if (my_item.get_rarity() == rarity::invalid || number_of_trials <= 0) { return; }

    std::mt19937 gen;
    for (int i = 0; i < number_of_trials; ++i)
    {
        if (my_item.get_lvl() == 10)
        {
            break;
        }
        my_item.attempt_upgrade();
        ++counts[my_item.get_lvl()];
    }
}

void output_map(std::map<int, int>& counts)
{
    std::cout << "The list of counts on each lvl" << std::endl;
    for (int i = 0; i < counts.size(); ++i)
    {
        std::cout << i << ": " << counts[i] << std::endl;
    }
}

int main()
{
    std::map<int, int> counts;
    std::string r;
    int number_of_trials;
    std::cin >> r >> number_of_trials;
    item my_item(string_to_rarity(r));

    find_expected_lvls_of_enchantments(counts, number_of_trials, my_item);
    output_map(counts);
}