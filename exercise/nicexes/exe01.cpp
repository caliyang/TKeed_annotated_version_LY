#include <iostream>
using namespace std;

class Complex
{
private:
    double m_real;
    double m_imag;
    int id;
    static int counter;

public:
    //    无参数构造函数
    Complex(void)
    {
        m_real = 0.0;
        m_imag = 0.0;
        id = (++counter);
        cout << "Complex(void):id=" << id << endl;
    }
    //    一般构造函数（也称重载构造函数）
    Complex(double real, double imag)
    {
        m_real = real;
        m_imag = imag;
        id = (++counter);
        cout << "Complex(double,double):id=" << id << endl;
    }
    //    复制构造函数（也称为拷贝构造函数）
    Complex(const Complex &c)
    {
        // 将对象c中的数据成员值复制过来
        m_real = c.m_real;
        m_imag = c.m_imag;
        id = (++counter);
        cout << "Complex(const Complex&):id=" << id << " from id=" << c.id << endl;
    }
    // 类型转换构造函数，根据一个指定的类型的对象创建一个本类的对象
    // explicit Complex(double r)
    Complex(double r)
    {
        m_real = r;
        m_imag = 0.0;
        id = (++counter);
        cout << "Complex(double):id=" << id << endl;
    }
    ~Complex()
    {
        cout << "~Complex():id=" << id << endl;
    }
    // 等号运算符重载
    Complex &operator=(const Complex &rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        this->m_real = rhs.m_real;
        this->m_imag = rhs.m_imag;
        cout << "operator=(const Complex&):id=" << id << " from id=" << rhs.id << endl;
        return *this;
    }
};

int Complex::counter = 0;

Complex test1(const Complex &c)
{
    return c;
}

Complex test2(const Complex c)
{
    return c;
}

Complex test3()
{
    static Complex c(1.0, 5.0);
    return c;
}

Complex &test4()
{
    static Complex c(1.0, 5.0);
    return c;
}

int main()
{
    cout << "00----------------00" << endl;
    Complex a, b;

    cout << "01----------------01" << endl;
    test1(a);
    test2(a);

    cout << "02----------------02" << endl;
    Complex c = test1(a);
    Complex d = test2(a);

    cout << "03----------------03" << endl;
    b = test3();
    b = test4();

    cout << "04----------------04" << endl;
    test2(1.2);
    test1(1.2);

    cout << "05----------------05" << endl;
    Complex e = test2(1.2);
    Complex f = test1(1.2);

    cout << "06----------------06" << endl;
    Complex g = test1(Complex(1.2));
    Complex h = test2(Complex(1.2));

    cout << "07----------------07" << endl;
    Complex i = Complex(1.2);
    Complex k(1.2);
    Complex j = 1.2;
    Complex l(Complex(1.2));

    cout << "08----------------08" << endl;
    return 0;
}

// 注意：
// 1.连着两次构造，会将第二次构造省略，直接将第一次构造形成的临时对象
//   视为最终构成的那个局部或全局对象；
// 2.函数参数传入或对象从函数返回为值传递时，视为“=”；