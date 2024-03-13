class Vector
{
    int x , y;
    public Vector(int x, int y)
    {
        this.x = x; this.y = y;
    }
    public Vector(int x)
    { 
        this.x = x; 
    }
    public Vector() { 
        this.x = 0;
        this.y = 0;
    }


    public static Vector operator -(Vector a, Vector b)
    {
        Vector result = new Vector();
        result.x = a.x - b.x;
        result.y= a.y - b.y;
        return result;
    }

    public static bool operator ==(Vector a, Vector b)
    {
        if (a.x == b.x && a.y == b.y)
            return true;
        return false;
    }
    public static bool operator !=(Vector a, Vector b)
    {
        if (a.x == b.x && a.y == b.y)
            return false;
        return true;
    }

    public static Vector operator *(Vector a, int n)
    {
        return new Vector(a.x * n, a.y * n);
    }

    public static Vector operator *(Vector a, Vector b)
    {

    }
}