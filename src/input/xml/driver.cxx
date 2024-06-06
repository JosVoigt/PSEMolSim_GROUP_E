#include <iostream>
#include <sstream>
#include "simulation-pskel.hxx"


class vector3D_pimpl: public vector3D_pskel
{
private:

    double x_{};
    double y_{};
    double z_{};
    std::string vectorType_;

public:

    vector3D_pimpl(const std::string& vectorType): vectorType_(vectorType) {}

    void x (double x) override
    {
        x_ = x;
    }

    void y (double y) override
    {
        y_ = y;
    }

    void z (double z) override
    {
        z_ = z;
    }


    void post_vector3D () override
    {
        std::cout << vectorType_ << ": X: " << x_ << ", Y: " << y_ << ", Z: " << z_ << std::endl;
    }
};


class cuboid_pimpl: public cuboid_pskel
{
private:

    vector3D_pimpl velocity_;
    vector3D_pimpl lowerLeftCorner_;
    double distance_{};
    double mass_{};
    int x_{};
    int y_{};
    int z_{};
    double brownianMotionMean_{};
    //vector3D_pimpl vector3d;

public:

    cuboid_pimpl(): velocity_("Velocity"), lowerLeftCorner_("LowerLeftCorner") {}

    void distance (double d) override
    {
        distance_ = d;
    }

    void mass (double m) override
    {
        mass_ = m;
    }

    void x (int x) override
    {
        x_ = x;
    }

    void y (int y) override
    {
        y_ = y;
    }

    void z (int z) override
    {
        z_ = z;
    }

    void brownianMotionMean (double bmm) override
    {
        brownianMotionMean_ = bmm;
    }

    void post_cuboid () override
    {
        std::cout << "Distance: " << distance_ << std::endl;
        std::cout << "Mass: " << mass_ << std::endl;
        std::cout << "X: " << x_ << std::endl;
        std::cout << "Y: " << y_ << std::endl;
        std::cout << "Z: " << z_ << std::endl;
        std::cout << "BrownianMotionMean: " << brownianMotionMean_ << std::endl;
    }
};


class lenjonesmol_pimpl: public lenjonesmol_pskel
{
private:
    double epsilon_{};
    double sigma_{};

public:
    void epsilon (double e) override
    {
        epsilon_ = e;
    }

    void sigma (double s) override
    {
        sigma_ = s;
    }

    void post_lenjonesmol () override
    {
        std::cout << "Epsilon: " << epsilon_ << std::endl;
        std::cout << "Sigma: " << sigma_ << std::endl;
    }
};


class simulation_pimpl: public simulation_pskel
{
private:
    double delta_{};
    int frequency_{};
    double start_{};
    double end_{};
    std::string outfile_;
    static cuboid_pskel cuboid_;
    std::vector<cuboid_pimpl> cuboids;

public:
    void delta (double d) override
    {
        delta_ = d;
    }

    void frequency (int f) override
    {
        frequency_ = f;
    }

    void start (double s) override
    {
        start_ = s;
    }

    void end (double e) override
    {
        end_ = e;
    }

    void outfile (const std::string& o) override
    {
        outfile_ = o;
    }

    void post_simulation () override
    {
        std::cout << "Delta: " << delta_ << std::endl;
        std::cout << "Frequency: " << frequency_ << std::endl;
        std::cout << "Start: " << start_ << std::endl;
        std::cout << "End: " << end_ << std::endl;
        std::cout << "Outfile: " << outfile_ << std::endl;
    }
};

using namespace std;

int
main (__attribute__((unused)) int argc, char* argv[])
{
    try
    {
        // Construct the parser.
        xml_schema::double_pimpl double_p;
        xml_schema::int_pimpl int_p;
        xml_schema::string_pimpl string_p;
        simulation_pimpl simulation_p;
        cuboid_pimpl cuboid_p;
        vector3D_pimpl velocity_p("Velocity");
        vector3D_pimpl lowerLeftCorner_p("LowerLeftCorner");
        lenjonesmol_pimpl lenjonesmol_p;

        //velocity
        velocity_p.x_parser(double_p);
        velocity_p.y_parser(double_p);
        velocity_p.z_parser(double_p);
        cuboid_p.velocity_parser(velocity_p);

        //lowerLeftCorner
        lowerLeftCorner_p.x_parser(double_p);
        lowerLeftCorner_p.y_parser(double_p);
        lowerLeftCorner_p.z_parser(double_p);
        cuboid_p.lowerLeftCorner_parser(lowerLeftCorner_p);

        //other cuboid parameters
        cuboid_p.distance_parser(double_p);
        cuboid_p.mass_parser(double_p);
        cuboid_p.x_parser(int_p);
        cuboid_p.y_parser(int_p);
        cuboid_p.z_parser(int_p);
        cuboid_p.brownianMotionMean_parser(double_p);
        simulation_p.cuboids_parser(cuboid_p);

        //other parameters
        simulation_p.delta_parser (double_p);
        simulation_p.frequency_parser (int_p);
        simulation_p.start_parser (double_p);
        simulation_p.end_parser (double_p);
        simulation_p.outfile_parser (string_p);

        //lenjonesmol
        lenjonesmol_p.epsilon_parser(double_p);
        lenjonesmol_p.sigma_parser(double_p);
        simulation_p.lenjonesmol_parser(lenjonesmol_p);


        // Parse the XML instance.
        xml_schema::document doc_p (simulation_p, "simulation");

        simulation_p.pre ();
        doc_p.parse (argv[1]);
        simulation_p.post_simulation ();
    }
    catch (const xml_schema::exception& e)
    {
        cerr << e << endl;
        return 1;
    }
}