import numpy_array_sample, cnoid_body_sample, cnoid_numpy_sample # sandbox
import BodyMethods
import numpy

def angleVector(body, av):
    '''
    set ith joint in body to ith element of av
    :param cnoid.Body body: body (ex. robot)
    :param numpy.ndarray/list av: list of joint angle. numpy.array of float64 is fastest
    '''
    if (not isinstance(av, numpy.ndarray)) or (av.dtype.type is not numpy.float64):
        av = numpy.array(av).astype(numpy.float64)
    BodyMethods.angleVector(body, av)
    return av
