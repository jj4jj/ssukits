#pragma once



template<class T,int dim >
class Vector
{
    typedef Vector<T,dim>   VecType;
public:
    Vector()
    {
        data.resize(dim);
    }
    //[0,dim)
    T operator [](int idx)
    {
        return data.at(idx);
    }
    /////////////////////////////
    VecType &  Zoom(int factor)
    {
        for(int i = 0;i < dim; ++i)
        {
            data[i] *= factor;
        }
        return *this;         
    }
    VecType & Move(const VecType & vec)
    {
        for(int i = 0;i < dim; ++i)
        {
            data[i] -= vec[i];
        }
        return *this;
    }
    //2D
    VecType & RotateX(double r)
    {
        
    }
    VecType & RotateY(double r)
    {
        
    }
    VecType & RotateZ(double r)
    {
        
    }
    T   InnerProduct()
    {
        
    }
    T   &   Length()
    {
        T   sum = 0;
        for(int i  = 0;i < dim ; ++i)
        {
            sum += data[i]*data[i];
        }        
        return sqrt(sum);
    }

public:
    std::vector<T>  data;
}



typedef Vector<double,2>    Vector2D;
typedef Vector<double,3>    Vector3D;









