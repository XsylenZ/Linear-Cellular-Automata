#include <iostream>
#include <string>
#include <vector>
#include <sstream>

const uint8_t RULE_A = 122;
const uint8_t RULE_B = 86;

const uint8_t POS_R = 0;
const uint8_t POS_L = 1;
const uint8_t POS_G = 2;

struct GenConfig
{
    int L = 0;
    int G = 0;
    uint8_t R = RULE_A;
};

/**
 * Get input tokens from std::cin.
 * @param [in,out] inputTokens
 */
std::vector<std::string> getInput();

/**
 * Sets parameters from given inputToken to the configuration of initial generation.
 * @param [in] inputTokens
 * @param [in,out] genConfig
 * @return boolean on success
 */
bool setGenConfig( const std::vector<std::string>& inputTokens, GenConfig& genConfig );

/**
 * Generates the initial generation from the given inputToken.
 * @param [in] inputTokens
 * @param [in,out] aGeneration
 * @return boolean on success
 */
bool initGeneration( const std::vector<std::string>& inputTokens, std::vector<bool>& aGeneration );

/**
 * Print the given generation.
 * @param [in] aGeneration
 */
void printGeneration( const std::vector<bool>& aGeneration );

/**
 * Updates given generation to the next generation.
 * @param [in,out] aGeneration
 * @param [in] rule
 */
void nextGeneration( std::vector<bool>& aGeneration, uint8_t rule );

int main()
{
    std::vector<std::string> inputTokens = getInput();

    GenConfig genConfig = {};
    assert( setGenConfig( inputTokens, genConfig ) );

    std::vector<bool> generation( genConfig.L );
    assert( initGeneration( inputTokens, generation ) );

    for ( auto i = 0; i < genConfig.G; ++i )
    {
        printGeneration( generation );
        nextGeneration( generation, genConfig.R );
    }

    return 0;
}

std::vector<std::string> getInput()
{
    std::string input;
    assert( getline( std::cin, input ) );

    std::string buf;
    std::stringstream ss( input );
    std::vector<std::string> inputTokens;

    while ( ss >> buf )
    {
        inputTokens.push_back( buf );
    }
    return inputTokens;
}

bool setGenConfig( const std::vector<std::string>& inputTokens, GenConfig& genConfig )
{
    switch ( inputTokens[POS_R][0] )
    {
        case 'A':
            genConfig.R = RULE_A;
            break;
        case 'B':
            genConfig.R = RULE_B;
            break;
        case 'U':
            genConfig.R = ( uint8_t ) std::stoi( inputTokens[3] );
            break;
        default:
            return false;
    }

    genConfig.L = std::stoi( inputTokens[POS_L] );
    genConfig.G = std::stoi( inputTokens[POS_G] );

    return true;
}

bool initGeneration( const std::vector<std::string>& inputTokens, std::vector<bool>& aGeneration )
{
    int L = std::stoi( inputTokens[POS_L] );
    auto it = std::find( inputTokens.begin(), inputTokens.end(), "init_start" );
    ++it;
    for ( ; it != inputTokens.end(); ++it )
    {
        if ( *it == "init_end" )
        {
            return true;
        }

        int pos = std::stoi( *it );

        if ( pos <= L )
        {
            aGeneration[pos - 1] = true;
        }
    }
    return false;
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

void nextGeneration( std::vector<bool>& aGeneration, uint8_t rule )
{
    std::vector<bool> v;
    for ( auto it = aGeneration.begin(); it != aGeneration.end(); ++it )
    {
        uint8_t pos = 0u | *( it - 1 ) << 2 | *( it ) << 1 | *( it + 1 );
        v.push_back( ( rule >> pos ) & 1u );
    }
    aGeneration = v;
}
