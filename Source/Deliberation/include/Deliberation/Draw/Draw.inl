namespace deliberation
{

template<typename T>
void Draw::setAttribute(const std::string & name, const T & value)
{
    setAttribute(name, Type::resolve<T>(), Blob::fromValue(value));
}

}
