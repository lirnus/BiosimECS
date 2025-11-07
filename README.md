# Biosim - ECS
by Linus Müller 2025<br>
This repo is a continuation of the [Evolution-Simulieren](https://github.com/MoritzBiosim/Evolution-Simulieren) project by Moritz Wagner and me.
Just as its predecessor, this project aims to observe evolution and adaptation of randomly generated populations under certain selective stresses.<br>
One of the limiting factors of the older version, which was written completely in Python, was its poor performance and thus scalability. The goal of this newer version is to implement all (or most) of the previous features while using more computationally efficient code. To do this, I switched programming languages from Python to C++ to gain more control over low-level allocation schedules and grant the compiler more opportunities for optimization.<br>
There is also a focus on readability of the code and interactivity for users (for example, simulation parameters should be changeable through an extern textfile, not inside the source code). The simulation also aims to be as portable as possible, requiring no external libraries (except for the rendering done in Python, which is optional).<br>
Last but not least, new features (like new neurons, environment settings, selection criteria, ...) should also be able to be added at any time in the future, which is why there is a heavy focus on modularity and cross-compability of all systems applied in the simulation.<br>

## The simulation
The principles of this simulation approach are adapted from David Miller's video [I programmed some creatures. They Evolved](https://www.youtube.com/watch?v=N3tRFayqVtk&t=1637s), to which the visualizations are also quite similar.<br>
### How it works
The basic principle of the Biosim is quite simple to understand and stems directly from natural biological systems: We simulate a population of creatures (*Pixies*) on a two-dimensional grid. These Pixies exhibit different behaviours based on their own *genome*, which gets randomly generated at the start of each simulation.<br>
The behaviour consists of simple patterns, which have the potential to become more and more complex the more genes/neurons are enabled. The basic structure of a Pixie "brain" consists of single "*neurons*" that execute certain functions and return values to activate other neurons or execute actions themselves. There are sensory neurons that can interpret the surroundings, like neighbourhood density or relative position in the world, which lead to action neurons that can for example exert a movement in a certain direction. From this large set of neurons, each Pixie gets assigned a subset of connected neurons that make up its brain.<br>
Pixie brains are generated randomly at the start of a simulation, the proceeding behaviour could look like this:
<p align="center">
    <img src=https://github.com/user-attachments/assets/fcf25bee-b827-44f2-9052-5a8dad314d96>
    <br>
    <em>One generation of a population of randomly generated pixies</em>
</p>

In the next step, a selection pressure gets applied on these pixies, causing some to perish (fitness = 0) and others to reproduce, getting chosen to pass on their genes to the next generation. The next generation gets filled up with individuals from the previous one, reaching the same size, but only choosing individuals with a fitness > 0. <br> Over many generations, we expect to see that Pixies with an appropriate behaviour survive and thrive, making up the biggest part of the population. <br>
This selection pressure could for example be to only select Pixies on the left half of the grid. In this case, we would expect to mostly find Pixies that exhibit a behaviour that more frequently leads to them being on the left half, and indeed that is what we find:<br>
<p align="center">
    <img src="https://github.com/user-attachments/assets/44ffe40f-d082-4010-bf29-fe369d3baf54">
    <br>
    <em>After only 20 generations, an adaptation response to the selection criterion "kill right half" has been found.</em>
</p>

This "kill right half" scenario is quite simple, but Biosim provides the framework to implement selection criteria as complex as one can imagine, from requiring the Pixies to consume food placed randomly on the grid, fleeing from predators or avoiding certain areas, to clump up, spread out or to overcome obstacles. Due to the heavy modularity of the simulation, these scenarios can always be added in later with no big effort.<br>
Also, there are multiple simulation modes that are thinkable/executable. First, we have the simple *"fixed"* population size with a selection criteria applied during or after the generation has finished. <br>
Secondly, there is a *"growing"* mode where Pixies duplicate live during a generation, with a reproduction cooldown timer that can be shortened by conducting certain actions (like eating food, touching barriers, ...). We experience exponential growth, and as soon as a certain threshold of Pixies is reached we reset the population size with Pixies picked randomly from the population. In principle, we also apply natural selection here, as Pixies who are more efficient at reducing their reproduction timer will grow faster and get picked more frequently. This simulation mode more closely represents how experimentalists would handle a lab culture of bacteria or yeast, which grow rapidly in a tube or flask and get "reset" when the culture has grown too much. <br>
Thirdly, there is a mode of *"continuous"* simulation, where Pixies can reproduce live during the simulation, but also die off after a certain amount of time. Here, a stable population size should arise after a while, dependent on the availability of resources like food and space. Here, we can also introduce "soft" selection criteria that shorten the lifespan of certain individuals or prolong the ones of others. In this setting, with the right simulation parameters, we can maybe experience the formation of a small ecosystem and witness interspecies interactions.<br>

#### Pixie Brains
As mentioned above, the behaviour of Pixies is randomized at the beginning of a simulation. But this doesn't mean the behaviour of one single Pixie is random; rather it is determined directly by its genome, which is in turn randomly generated. A Pixie genome is a collection of 32bit-integer genes, of which each one encodes for a connection in the brain

### Effects to be studied
The simulation realizes concepts like fitness, mutation, gene drift, adaptation, migration, sexual reproduction etc.<br>
Applied to population genetics, we can study the effect of these concepts on adaptation and mean fitness of a population. With the right analysis, we can extract single gene fitness values, draw fitness landscapes or compute gene epistasis effects.

## Experimental and Analytical features
The Biosim has multiple features that enable a highly controlled experimental setup, detailed analyis and maximum reproducibility.<br>

#### Setup features:
- All parameters are set thorugh a textfile called simconfig.ini ; the metadata including all set parameters is saved and printed for every simulation run
- A random seed can be set at the start of the simulation to guarantee reproducible results
- The *metagenome* of a previously simulated generation can be fed back into the program as a starting population (through this, you can also simulate **clonal** populations of "established"(=already adapted) populations)
- single Neurons can be manually dis- or enabled to reduce or increase complexity in behaviour
- different environments (grids with obstacles, dispersion of food) can be chosen through a key in the simconfig textfile

#### Analysis features:
- The *metagenome* of each generation can be saved in a textfile, which allows for analysis on a population genetics level (see *Gene Frequencies*, *Mullerplot*) and visualization of Pixie brains
- The positions and movements of Pixies can be saved to render GIFs (as shown above)
- The survival rate or rather the mean fitness and the approximate diversity of a population can be saved and rendered

#### Gallery
Collection of example plots from different simulation runs, illustrating the kind of data analysis that can be done
<p align="center">
    <img width="640" height="235" alt="pixiebrain2" src="https://github.com/user-attachments/assets/75125828-4705-4676-9f91-709007b38d13" />
    <br>
    <em>Visualization of a single Pixie brain (only neurons with connections are plotted)</em>
</p>
<p align="center">
    <img width="640" height="480" alt="survival_diversity_plot" src="https://github.com/user-attachments/assets/18bc0b9f-8bc9-4e0d-920a-91da0b633751" />
    <br>
    <em>Example of survival rates and diversity across generations</em>
</p>
<p align="center">
    <img width="640" height="480" alt="mullerplot1" src="https://github.com/user-attachments/assets/b9a7d91b-e3cc-49c7-8e37-94c32c561e40" />
    <br>
    <em>A "Mullerplot" of one simulation run, showing which strands are successful at which times</em>
</p>
<p align="center">
    <img width="640" height="480" alt="gene_frequencies" src="https://github.com/user-attachments/assets/099abca2-b5a1-4220-bdf0-43399887b86c" />
    <br>
    <em>Gene Frequencies (=abundance in the population) plotted for every gene. Here you can clearly see which genes spread in the population (ascending lines), which genes lie on the same genome (parallel lines) and which genes are sorted out when a fitter variant arises through mutation (line dipping from a bundle of parallel lines).</em>
</p>
