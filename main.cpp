#include <iostream>
#include <vector>

const uint8_t RULE_A = 0x7A; //01111010
const uint8_t RULE_B = 0x56; //01010110

struct GenConfig
{
    char type = ' ';
    int L = 0;
    int G = 0;
    uint16_t rule = 0x00;
    std::vector<uint8_t> occupiedPositions;
};

/**
 * Populates the config with the user input values, according to the following syntax:
 * [A,B or U <1-255>] L G init_start X[n] init_stop
 * @param [in,out] config
 * @return Input failed/succeeded
 */
bool setConfigFromUserInput( GenConfig& config );

/**
 * Initializes the first generation with the occupiedPositions from the given config
 * @param [in] config
 * @param [in,out] aGeneration
 */
void initGeneration( const GenConfig& config, std::vector<bool>& aGeneration );

/**
 * Print the given generation
 * @param [in] aGeneration
 */
void printGeneration( const std::vector<bool>& aGeneration );

/**
 * Updates the given generation, with new values, based on the given rule and previous
 * values of the given generation.
 * @param [in] rule
 * @param [in,out] aGeneration
 */
void nextGeneration( uint8_t rule, std::vector<bool>& aGeneration );

int main()
{
    GenConfig genConfig = {};
    if ( !setConfigFromUserInput( genConfig ) )
    {
        return -1;
    }

    std::vector<bool> generation( genConfig.L );
    initGeneration( genConfig, generation );

    for ( auto i = 0; i < genConfig.G; ++i )
    {
        printGeneration( generation );
        nextGeneration( genConfig.rule, generation );
    }

    return 0;
}

bool setConfigFromUserInput( GenConfig& config )
{
    std::cout << "Enter automaton description:" << std::endl;
    std::cin >> config.type;
    switch ( config.type )
    {
        case 'A':
            config.rule = RULE_A;
            break;
        case 'B':
            config.rule = RULE_B;
            break;
        case 'U':
            int ruleU;
            std::cin >> ruleU;
            if ( ruleU < 0 || ruleU > 255 )
            {
                std::cout << "Argument value for U must be 1 to 255" << std::endl;
                return false;
            }
            config.rule = ( uint8_t ) ruleU;
            break;
        default:
            std::cout << "Invalid argument for automaton type. Use: A, B or U" << std::endl;
            return false;
    }

    std::cin >> config.L >> config.G;
    if ( config.L < 1 )
    {
        std::cout << "Argument for L needs to be a positive integer bigger then 1" << std::endl;
        return false;
    }
    if ( config.G < 1 )
    {
        std::cout << "Argument for G needs to be a positive integer bigger then 1" << std::endl;
        return false;
    }

    std::string initStart;
    std::cin >> initStart;
    if ( initStart != "init_start" )
    {
        std::cout << "Invalid argument: \'" << initStart << "\'" << std::endl;
        return false;
    }

    int number;
    //Break when input is not a number anymore, IE 'init_end'
    while ( std::cin >> number )
    {
        if ( number > 1 && number < config.L )
        {
            // Minus one, to translate given numbers to index numbers
            config.occupiedPositions.push_back( number - 1 );
        }
    }

    if ( config.occupiedPositions.empty() )
    {
        std::cout << "Need at least one occupied position for init generation" << std::endl;
        return false;
    }

    return true;
}

void initGeneration( const GenConfig& config, std::vector<bool>& aGeneration )
{
    for ( uint8_t occupiedPosition: config.occupiedPositions )
    {
        if ( occupiedPosition <= config.L )
        {
            aGeneration[occupiedPosition] = true;
        }
    }
}

void nextGeneration( uint8_t rule, std::vector<bool>& aGeneration )
{
    std::vector<bool> v;
    for ( auto it = aGeneration.begin(); it != aGeneration.end(); ++it )
    {
        uint8_t pos = 0u | *( it - 1 ) << 2 | *( it ) << 1 | *( it + 1 );
        v.push_back( ( rule >> pos ) & 1u );
    }
    aGeneration = v;
}

void printGeneration( const std::vector<bool>& aGeneration )
{
    for ( bool i: aGeneration )
    {
        if ( i )
        {
            std::cout << '*';
        }
        else
        {
            std::cout << ' ';
        }
    }
    std::cout << std::endl;
}
