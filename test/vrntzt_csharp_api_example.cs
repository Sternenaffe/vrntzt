/*using System;
using System.Text;
using System.Linq;

namespace vrntzt.neat.csharp_api.test
{ 
    public static class NeatXor
    {
        public static float EvalXor(SimplisticGenotype t_genotype)
        {
            float fitness = 0;
            SimplisticPhenotype phenotype = t_genotype.Decode();

            try
            {
                fitness += 1 - _neatXor(phenotype, 0, 0);
                fitness += _neatXor(phenotype, 0, 1);
                fitness += _neatXor(phenotype, 1, 0);
                fitness += 1 - _neatXor(phenotype, 1, 1);
            }
            finally
            {
                phenotype.Dispose();
            }

            return fitness;
        }

        private static float _neatXor(SimplisticPhenotype t_phenotype,
            float t_in1, float t_in2)
        {
            t_phenotype.Reset();
            t_phenotype.SetInput(0, t_in1);
            t_phenotype.SetInput(1, t_in2);
            t_phenotype.Activate(10);

            return t_phenotype.GetOutput(0);
        }
    }

    public class Program
    {

        /// <summary>
        /// output callback function
        /// </summary>
        internal static void OutputCallbackFunction(StringBuilder t_string, uint t_len)
        {
            Console.WriteLine(t_string);
        }

        static void Main(string[] args)
        {
            // set output callbacks
            IO.SetAllStreams(OutputCallbackFunction);

            Random random = new Random();

            NeatEvolutionSettings settings = new NeatEvolutionSettings()
            {
                PopulationSize = 100,
                SpeciesCount = 10,
                InterspeciesMatingChance = 0.001f
            };

            NeatEvolutionManager manager = new NeatEvolutionManager(2, 1, settings);

            // starting population
            manager.CreateRandomPopulation();

            uint generation = 0;

            manager.Save("test.xml");
            manager.Load("test.xml");
            manager.Save("test_com.xml");

            while (true)
            {
                manager.GetPreviousBestGenotype();

                Console.WriteLine($"\n============== Generation {++generation} ==============");

                // eval fitness
                foreach (var individual in manager.GetPopulation())
                {
                    individual.SetFitness(NeatXor.EvalXor(individual));
                }

                manager.EvolvePopulation();
            }
        }
    }
}
*/

using System;
using vrntzt.neat.csharp_api;

class Program
{
    static void Main()
    {
        // Settings for Evolution
        NeatEvolutionSettings settings = new NeatEvolutionSettings()
        {
            PopulationSize = 100,
            SpeciesCount = 10,
            InterspeciesMatingChance = 0.001f
        };

        // create evolution manager
        NeatEvolutionManager manager = new NeatEvolutionManager(2, 1, settings);

        // try to load population, if no file exists: create starting population
        try
        {
            manager.Load("population.xml");
        }
        catch
        {
            manager.CreateRandomPopulation();
        }

        uint generation = 0;

        while (true)
        {
            // print generations best fitness via acquiring of best genotype
            float bestFitness = manager.GetPreviousBestGenotype().GetFitness();
            Console.WriteLine($"Best fitness: {bestFitness}");

            Console.WriteLine($"\n============== Generation {++generation} ==============");

            // evaluate fitness of networks
            foreach (SimplisticGenotype genotype in manager.GetPopulation())
            {
                // decode genotype into phenotype
                SimplisticPhenotype phenotype = genotype.Decode();

                // set inputs – GetNextInput() is custom function
                phenotype.SetInput(0, GetNextInput());
                phenotype.SetInput(1, GetNextInput());

                // Reset because should be combinatorial network
                phenotype.Reset();

                // in this case, the (arbitrary) problem doesn’t require internal states, so
                // iterations should be kept low
                phenotype.Activate(3);

                float output = phenotype.GetOutput(0);

                // CalculateFitness is a custom function
                float fitness = CalculateFitness(output);
                genotype.SetFitness(fitness);

                // phenotype is not needed anymore – should be done with try – catch in an
                // actual program
                phenotype.Dispose();
            }

            // advance evolution by one generation
            manager.EvolvePopulation();

            // save population every 100 generations
            if (generation % 100 == 0)
            {
                manager.Save("population.xml");
            }
        }
    }
}
